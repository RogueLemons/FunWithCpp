#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <array>
#include <charconv>
#include <limits>

static void print_help_message()
{
    std::cerr << "Input must be -f or --file followed by a .csv filename" << std::endl;
}

static bool valid_input(int argc, char* argv[])
{
    if (argc != 3)
    {
        print_help_message();
        return false;
    }

    std::string option = argv[1];
    std::string filename = argv[2];

    if (option != "-f" && option != "--file")
    {
        print_help_message();
        return false;
    }

    if (filename.size() < 5 || filename.substr(filename.size() - 4) != ".csv")
    {
        print_help_message();
        return false;
    }

    std::filesystem::path file(filename);
    if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file))
    {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }

    return true;
}

static std::vector<std::string> read_file_lines(const std::filesystem::path& file)
{
    std::vector<std::string> lines;
    std::ifstream in(file);

    if (!in.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(in, line))
    {
        lines.push_back(line);
    }

    if (lines.size() == 0)
    {
        std::cerr << "File is empty" << std::endl;
    }
    return lines;
}

static bool valid_csv_line(const std::string& line)
{
    std::size_t first_comma = line.find(',');
    if (first_comma == std::string::npos)
    {
        return false;
    }

    std::size_t second_comma = line.find(',', first_comma + 1);
    if (second_comma == std::string::npos)
    {
        return false;
    }

    if (line.find(',', second_comma + 1) != std::string::npos)
    {
        return false;
    }

    std::string first  = line.substr(0, first_comma);
    std::string second = line.substr(first_comma + 1,
                                     second_comma - first_comma - 1);
    std::string third  = line.substr(second_comma + 1);

    if (first.empty() || second.empty() || third.empty())
    {
        return false;
    }

    return true;
}

static std::optional<std::tm> parse_timestamp(const std::string& timestamp)
{
    std::tm tm = {};

    std::istringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail())
    {
        return std::nullopt;
    }

    return tm;
}

static std::array<std::string, 3> split_csv_three(const std::string& line)
{
    std::array<std::string, 3> values;

    std::size_t first_comma = line.find(',');
    std::size_t second_comma = line.find(',', first_comma + 1);

    values[0] = line.substr(0, first_comma);

    values[1] = line.substr(
        first_comma + 1,
        second_comma - first_comma - 1
    );

    values[2] = line.substr(second_comma + 1);

    return values;
}

struct temperature_file_data
{
    std::string first_line{};
    std::vector<std::string> error_lines{};
    std::map<std::string, std::vector<std::array<std::string, 3>>> sensor_readings{};
};

temperature_file_data read_temperature_strings(const std::vector<std::string>& file_lines)
{
    auto result = temperature_file_data{};
    result.first_line = file_lines.at(0);

    for (int i = 1; i < file_lines.size(); i++)
    {
        const std::string& line = file_lines[i];
        if (!valid_csv_line(line))
        {
            result.error_lines.push_back(line);
            continue;
        }
        std::array<std::string, 3> values = split_csv_three(line);
        std::string id = values.at(1);
        result.sensor_readings[id].push_back(values);
    }

    return result;
}

static bool is_older(std::tm lhs, std::tm rhs)
{
    std::time_t lhs_time = std::mktime(&lhs);
    std::time_t rhs_time = std::mktime(&rhs);

    return lhs_time < rhs_time;
}

static std::optional<double> parse_double(const std::string& s)
{
    double value = 0.0;

    const char* begin = s.data();
    const char* end   = s.data() + s.size();

    auto result = std::from_chars(begin, end, value);

    // Must succeed AND consume entire string
    if (result.ec != std::errc{} || result.ptr != end)
    {
        return std::nullopt;
    }

    return value;
}

struct sensor_data
{
    std::string sensor_id{};
    std::size_t readings{};
    double average{};
    double min{};
    double max{};
    std::vector<std::string> anomalies{};
};

sensor_data compute_sensor_data(const std::string& sensor_id, const std::vector<std::array<std::string, 3>>& values)
{
    sensor_data res{};
    res.sensor_id = sensor_id;
    res.readings = values.size();
    
    // Average, min, max
    res.min = std::numeric_limits<double>::max();
    res.max = std::numeric_limits<double>::lowest();;
    double sum = 0.0;
    std::size_t counted_temps = 0;
    for (const std::array<std::string, 3>& v : values)
    {
        std::optional<double> temp = parse_double(v[2]);
        if (temp.has_value())
        {
            sum += temp.value();
            counted_temps++;

            if (temp < res.min)
            {
                res.min = temp.value();
            }
            if (temp > res.max)
            {
                res.max = temp.value();
            }
        }
    }
    if (values.empty())
    {
        res.average = std::numeric_limits<double>::quiet_NaN();
        res.min = std::numeric_limits<double>::quiet_NaN();
        res.max = std::numeric_limits<double>::quiet_NaN();
    }
    else 
    {
        res.average = sum / counted_temps;
    }

    // Find anomalies
    std::tm prev_time{};
    prev_time.tm_year = -1900; // year 0
    prev_time.tm_mon  = 0;     // January
    prev_time.tm_mday = 1;
    for (const std::array<std::string, 3>& v : values)
    {
        std::optional<std::tm> time = parse_timestamp(v[0]);
        std::optional<double> temp = parse_double(v[2]);
        std::string reconstructed_line = v[0] + ',' + v[1] + ',' + v[2];

        if (!time.has_value() || is_older(time.value(), prev_time))
        {
            res.anomalies.push_back(reconstructed_line);
            continue;
        }
        if (!temp.has_value())
        {
            res.anomalies.push_back(reconstructed_line);
            continue;
        }
        bool temp_too_big = (temp.value() >= (res.average + 10.0));
        bool temp_too_low = (temp.value() <= (res.average - 10.0)); 
        if (temp_too_big || temp_too_low)
        {
            res.anomalies.push_back(reconstructed_line);
            continue;
        }

        prev_time = time.value();
    }

    return res;
}

std::vector<sensor_data> compute_all_sensor_data(const temperature_file_data& data)
{
    std::vector<sensor_data> res{};
    for (const auto& [key, value] : data.sensor_readings)
    {
        res.push_back(compute_sensor_data(key, value));
    }
    return res;
}

void print_sensor_data(const sensor_data& data)
{
    std::string anomalies_print{};
    if (data.anomalies.size() == 0)
    {
        anomalies_print = "none";
    }
    else
    {
        for (const auto& anomaly : data.anomalies)
        {
            anomalies_print += "\n    " + anomaly;
        }
    }

    std::cout << std::fixed << std::setprecision(1) 
    << "Sensor: " << data.sensor_id
    << "\n  Readings : " << data.readings
    << "\n  Average  : " << data.average
    << "\n  Min      : " << data.min
    << "\n  Max      : " << data.max
    << "\n  Anomalies: " << anomalies_print << std::endl;
}

void print_error_input(const std::vector<std::string>& error_lines)
{
    if (error_lines.size() == 0)
    {
        std::cout << "Error input: None" << std::endl; 
    }
    else
    {
        std::cout << "Error input:" << std::endl;
        for (const std::string& line : error_lines)
        {
            std::cout << "  " << line << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (!valid_input(argc, argv))
    {
        return 1;
    }

    std::filesystem::path file(argv[2]);
    std::vector<std::string> file_lines = read_file_lines(file);
    if (file_lines.size() == 0)
    {
        return 2;
    }

    temperature_file_data temperature_data = read_temperature_strings(file_lines);
    std::vector<sensor_data> sensors = compute_all_sensor_data(temperature_data);
    if (sensors.size() == 0)
    {
        return 3;
    }

    for (const sensor_data& sensor : sensors)
    {
        print_sensor_data(sensor);
        std::cout << std::endl;
    }
    print_error_input(temperature_data.error_lines);

    return 0;
}
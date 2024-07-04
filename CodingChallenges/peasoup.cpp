// https://open.kattis.com/problems/peasoup

#include <iostream>
#include <string>
#include <vector>
using std::string, std::vector, std::getline;

vector<string> get_lines() {
    vector<string> lines;
    string line;
    while (getline(std::cin, line)) {
        lines.push_back(line);
    }
    return lines;
}

string get_restaurant(vector<string> lines) {
    int restaurants_count = std::stoi(lines.front());
    int restaurant_index = 0;
    int index = 1;
    while (restaurant_index < restaurants_count) {
        int items = std::stoi(lines[index]);
        index++;
        string name = lines[index];
        index++;
        bool has_soup = false;
        bool has_pancakes = false;
        for (int i = 0; i < items; i++) {
            string item = lines[index];
            if (item == "pea soup")
                has_soup = true;
            if (item == "pancakes")
                has_pancakes = true;
            index++;
        }

        if (has_soup && has_pancakes) {
            return name;
        }
        restaurant_index++;
    }
    return "Anywhere is fine I guess";
}

int main(void) {
    auto lines = get_lines();
    auto restaurant = get_restaurant(lines);

    std::cout << restaurant;

    return 0;
}


int alternative_main(void) {

    string line;
    getline(std::cin, line);
    int number_of_restaurants = std::stoi(line);

    for (int i = 0; i < number_of_restaurants; i++) {
        getline(std::cin, line);
        int options = std::stoi(line);

        string name;
        getline(std::cin, name);

        bool has_soup = false;
        bool has_pancakes = false;
        for (int i = 0; i < options; i++) {
            string food_option;
            getline(std::cin, food_option);
            if (food_option == "pea soup")
                has_soup = true;
            if (food_option == "pancakes")
                has_pancakes = true;
        }

        if (has_soup && has_pancakes) {
            std::cout << name;
            return 0;
        }
    }
    std::cout << "Anywhere is fine I guess";
    return 0;
}
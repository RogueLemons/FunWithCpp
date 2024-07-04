// https://open.kattis.com/problems/peasoup

#include <iostream>
#include <string>

int main(void) {

    std::string line;
    std::getline(std::cin, line);
    int number_of_restaurants = std::stoi(line);

    for (int i = 0; i < number_of_restaurants; i++) {
        std::getline(std::cin, line);
        int options = std::stoi(line);

        std::string name;
        std::getline(std::cin, name);

        bool has_soup = false;
        bool has_pancakes = false;
        for (int i = 0; i < options; i++) {
            std::string food_option;
            std::getline(std::cin, food_option);
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
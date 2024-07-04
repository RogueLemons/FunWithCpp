// https://open.kattis.com/problems/dvds

#include <iostream>
#include <vector>

std::vector<std::vector<int>> read_dvd_stacks() {
    long long data;
    int i = -1;
    int number_of_stacks = 0;
    int number_of_dvds = 10;
    std::vector<std::vector<int>> stacks;
    while (std::cin >> data) {
        if (i == -1) {
            number_of_stacks = data;
            i++;
            continue;
        }
        if (i == 0) {
            number_of_dvds = data;
            stacks.push_back(std::vector<int>{});
        }
        else {
            stacks.back().push_back(data);
        }
        i++;
        if (i > number_of_dvds) {
            i = 0;
        }
    }
    return stacks;
}

int lowest_number_of_operations_to_sort_stack(const std::vector<int>& dvd_stack) {
    int operations = 0;
    int index = 0;
    for (auto dvd : dvd_stack) {
        index++;
        if (index - operations != dvd)
            operations++;
    }
    return operations;
}

std::vector<int> lowest_number_of_operations_to_sort_stacks(const std::vector<std::vector<int>>& dvd_stacks) {
    std::vector<int> operation_counts;
    for (const auto& stack : dvd_stacks) {
        operation_counts.push_back(lowest_number_of_operations_to_sort_stack(stack));
    }
    return operation_counts;
}

void print_operation_counts(const std::vector<int>& operation_counts) {
    for (auto operations : operation_counts) {
        std::cout << operations << std::endl;
    }
}

int main(void) {
    const auto dvd_stacks = read_dvd_stacks();
    const auto operation_counts = lowest_number_of_operations_to_sort_stacks(dvd_stacks);
    print_operation_counts(operation_counts);
    return 0;
}
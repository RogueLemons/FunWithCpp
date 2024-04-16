// VectorImplementation.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "MyVector.hpp"
using std::cout;
using std::endl;

static void test_my_simple_vector();
static void test_my_modular_vector();

template <typename T>
static void print(const my::Vector<T>& vec) {
    cout << "Size: " << vec.size() << ", Cap: " << vec.capacity();
    cout << ", Elements: [";
    for (int i = 0; i < vec.size(); i++) {
        cout << " " << vec[i];
        if (i != vec.size() - 1)
            cout << ",";
    }
    cout << " ] " << endl << endl;
}

int main()
{
    try {
        test_my_simple_vector();
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

void test_my_simple_vector()
{
    cout << "\n--- Simple Vector with Manual Capacity Resizing  ---" << endl << endl;

    int initial_capacity = 5;
    my::Vector<double> vec(initial_capacity);
    cout << "Empty vector with initial capacity " << initial_capacity << ": " << endl;
    print(vec);

    cout << "Add some values:" << endl;
    vec.push_back(1.2);
    vec.push_back(2.4);
    vec.push_back(3.6);
    vec.push_back(4.8);
    vec.push_back(6.0);
    vec.push_back(7.2);
    print(vec);

    int index = 2;
    cout << "Erase at index " << index << ":" << endl;
    vec.erase(index);
    print(vec);

    index = 1;
    cout << "Insert at index " << index << ":" << endl;
    vec.insert(index, 7.7);
    print(vec);

    index = 8;
    while (vec.size() != vec.capacity())
        vec.push_back(42);
    double value = 7.7;
    cout << "Insert " << value << " at index " << index << " (with capacity increase) : " << endl;
    vec.insert(index, 7.7);
    print(vec);

    cout << "Get out-of-bounds elements " << -1 << ", " << vec.size() << ", " << 2 * vec.size() + 3 << ":" << endl;
    cout << vec[-1] << ", " << vec[vec.size()] << ", " << vec[2 * vec.size() + 3] << endl << endl;

    while (vec.size() > 3)
        vec.pop_back();
    cout << "Pop values until size is 3" << endl;
    print(vec);

    cout << "Free up memory by reducing capacity:" << endl;
    vec.adjust_capacity();
    print(vec);
}

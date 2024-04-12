// VectorImplementation.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "MyVector.hpp"
using std::cout;
using std::endl;

static void print(const my::Vector<double>& vec) {
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
        cout << "--- Vector Implementation ---" << endl << endl;
        my::Vector<double> vec;

        cout << "Empty vector:" << endl;
        print(vec);

        cout << "Add some values:" << endl;
        vec.push_back(1.2);
        vec.push_back(2.4);
        vec.push_back(3.6);
        vec.push_back(4.8);
        vec.push_back(6.0);
        print(vec);

        int index = 3;
        cout << "Erase at index " << index << ":" << endl;
        vec.erase(index);
        print(vec);

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

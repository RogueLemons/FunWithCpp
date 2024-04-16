#pragma once

namespace {
	template <typename T>
	constexpr static void replace_array(T*& elements, int size, int& capacity) {
		T* new_elements = new T[capacity];
		for (int i = 0; i < size; i++) {
			new_elements[i] = elements[i];
		}
		delete[] elements;
		elements = new_elements;
	}
}

namespace cap_adj {

	template <typename T>
	struct Continous {
		constexpr static void adjust_capacity(T*& elements, int size, int& capacity) {
			if (size == capacity || size < capacity / 3) {
				capacity = std::max(3, size * 2);
				replace_array(elements, size, capacity);
			}
		}
	};

	template <typename T>								// w/ C++ 20: <typename T, IncreaseFactor F = {}> for even more modularity, where IncreaseFactor is a struct containing a float factor
	struct IncreaseOnly {
		constexpr static void adjust_capacity(T*& elements, int size, int& capacity) {
			if (size == capacity) {
				capacity = capacity * 1.5f;				// w/ C++20: * F.factor;
				replace_array(elements, size, capacity);
			}
		}
	};

}
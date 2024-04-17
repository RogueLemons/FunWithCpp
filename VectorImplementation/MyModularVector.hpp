#pragma once

#include <stdexcept>
#include "CapacityModules.hpp"

namespace my_modular {
	template <typename T, typename CapStrat>		// Consider requires std::derived_from<ICapacityStrategy> CapStrat
	class Vector;
}

template <typename T, typename CapStrat>
class my_modular::Vector
{
private:
	// Fields
	int _size;
	int _capacity;
	T* _elements;

	// Helper methods
	void adjust_memory();

public:
	// Constructors and Destructor
	Vector(int initial_capacity = 3);
	Vector(int size, const T& inital_values);
	Vector(const Vector& source);
	~Vector();

	// Methods (one-liners)
	int size() const { return _size; }
	int capacity() const { return _capacity; }
	bool is_empty() const { return _size == 0; }
	T& back() const { return _elements[_size - 1]; }
	T& front() const { return _elements[0]; }

	// Methods
	void push_back(const T& element);
	void pop_back();
	void insert(int index, const T& element);
	void erase(int index);
	void clear();

	// Operators
	T& operator [](int index) { return _elements[index]; }
	const T& operator [](int index) const { return _elements[index]; }
	Vector& operator =(const Vector& source);
};

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::adjust_memory()
{
	if (CapStrat::adjust_capacity(_size, _capacity)) {
		T* new_elements = new T[_capacity];
		for (int i = 0; i < _size; i++) {
			new_elements[i] = _elements[i];
		}
		delete[] _elements;
		_elements = new_elements;
	}
}

template<typename T, typename CapStrat>
inline my_modular::Vector<T, CapStrat>::Vector(int initial_capacity)
{
	_size = 0;
	_capacity = initial_capacity;
	_elements = new T[_capacity];
}

template<typename T, typename CapStrat>
inline my_modular::Vector<T, CapStrat>::Vector(int size, const T& inital_values)
{
	_size = size;
	_capacity = size * 2;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = inital_values;
	}
}

template<typename T, typename CapStrat>
inline my_modular::Vector<T, CapStrat>::Vector(const Vector& source)
{
	_size = source._size;
	_capacity = source._capacity;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = source._elements[i];
	}
}

template<typename T, typename CapStrat>
inline my_modular::Vector<T, CapStrat>::~Vector()
{
	delete[] _elements;
}

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::push_back(const T& element)
{
	adjust_memory();

	_elements[_size] = element;
	_size++;
}

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::pop_back()
{
	if (_size > 0) {
		_size--;
	}

	adjust_memory();
}

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::insert(int index, const T& element)
{
	adjust_memory();

	for (int i = _size; i >= index; i--) {
		_elements[i] = _elements[i - 1];
	}
	_elements[index] = element;
	_size++;
}

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::erase(int index)
{
	_size--;
	for (int i = index; i < _size; i++) {
		_elements[i] = _elements[i + 1];
	}

	adjust_memory();
}

template<typename T, typename CapStrat>
inline void my_modular::Vector<T, CapStrat>::clear()
{
	_size = 0; 
	adjust_memory();
}

template<typename T, typename CapStrat>
inline my_modular::Vector<T, CapStrat>& my_modular::Vector<T, CapStrat>::operator=(const my_modular::Vector<T, CapStrat>& source)
{
	if (source._size > _size) {
		delete[] _elements;
		_capacity = source._size * 2;
		_elements = new T[_capacity];
	}

	_size = source._size;
	for (int i = 0; i < _size; i++) {
		_elements[i] = source._elements[i];
	}

	return *this;
}



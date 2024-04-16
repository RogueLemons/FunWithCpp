#pragma once

#include <stdexcept>
#include "CapacityModules.hpp"

namespace my_modular {
	template <typename T, typename A>
	class Vector;
}

template <typename T, typename A>
class my_modular::Vector
{
private:
	// Fields
	int _size;
	int _capacity;
	T* _elements;

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
	void clear() { _size = 0; A::adjust_capacity(_elements, _size, _capacity); }

	// Methods
	void push_back(const T& element);
	void pop_back();
	void insert(int index, const T& element);
	void erase(int index);

	// Operators
	T& operator [](int index) { return _elements[index]; }
	const T& operator [](int index) const { return _elements[index]; }
	Vector& operator =(const Vector& source);
};

template<typename T, typename A>
inline my_modular::Vector<T, A>::Vector(int initial_capacity)
{
	_size = 0;
	_capacity = initial_capacity;
	_elements = new T[_capacity];
}

template<typename T, typename A>
inline my_modular::Vector<T, A>::Vector(int size, const T& inital_values)
{
	_size = size;
	_capacity = size * 2;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = inital_values;
	}
}

template<typename T, typename A>
inline my_modular::Vector<T, A>::Vector(const Vector& source)
{
	_size = source._size;
	_capacity = source._capacity;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = source._elements[i];
	}
}

template<typename T, typename A>
inline my_modular::Vector<T, A>::~Vector()
{
	delete[] _elements;
}

template<typename T, typename A>
inline void my_modular::Vector<T, A>::push_back(const T& element)
{
	A::adjust_capacity(_elements, _size, _capacity);

	_elements[_size] = element;
	_size++;
}

template<typename T, typename A>
inline void my_modular::Vector<T, A>::pop_back()
{
	if (_size > 0) {
		_size--;
	}

	A::adjust_capacity(_elements, _size, _capacity);
}

template<typename T, typename A>
inline void my_modular::Vector<T, A>::insert(int index, const T& element)
{
	A::adjust_capacity(_elements, _size, _capacity);

	for (int i = _size; i >= index; i--) {
		_elements[i] = _elements[i - 1];
	}
	_elements[index] = element;
	_size++;
}

template<typename T, typename A>
inline void my_modular::Vector<T, A>::erase(int index)
{
	_size--;
	for (int i = index; i < _size; i++) {
		_elements[i] = _elements[i + 1];
	}

	A::adjust_capacity(_elements, _size, _capacity);
}

template<typename T, typename A>
inline my_modular::Vector<T, A>& my_modular::Vector<T, A>::operator=(const my_modular::Vector<T, A>& source)
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



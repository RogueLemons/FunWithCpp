#pragma once

#include <stdexcept>

namespace my {
	template <typename T>
	class Vector;
}

template <typename T>
class my::Vector
{
private:
	// Fields
	int _size;
	int _capacity;
	T* _elements;

	// Out-of-bounds indexing
	int adjust_index(int index) const;

public:
	// Constructors and Destructor
	Vector(int initial_capacity = 10);
	Vector(int size, const T& inital_values);
	Vector(const Vector& source);
	~Vector();

	// Methods (one-liners)
	int size() const { return _size; }
	int capacity() const { return _capacity; }
	bool is_empty() const { return _size == 0; }
	T& back() const { return _elements[_size - 1]; }
	T& front() const { return _elements[0]; }
	void clear() { _size = 0; }

	// Methods
	void push_back(const T& element);
	void pop_back();
	void insert(int index, const T& element);
	void erase(int index);
	void adjust_capacity();
	void adjust_capacity(int capacity);

	// Operators
	T& operator [](int index) { return _elements[adjust_index(index)]; }
	const T& operator [](int index) const { return _elements[adjust_index(index)]; }
	Vector& operator =(const Vector& source);
};

template<typename T>
inline int my::Vector<T>::adjust_index(int index) const
{
	if (is_empty())
		throw std::invalid_argument("Cannot access elements of empty Vector.");
	else if (index >= _size)
		return adjust_index(index - _size);
	else if (index < 0)
		return adjust_index(_size + index);
	else
		return index;
}

template<typename T>
inline my::Vector<T>::Vector(int initial_capacity)
{
	_size = 0;
	_capacity = initial_capacity;
	_elements = new T[_capacity];
}

template<typename T>
inline my::Vector<T>::Vector(int size, const T& inital_values)
{
	_size = size;
	_capacity = size * 2;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = inital_values;
	}
}

template<typename T>
inline my::Vector<T>::Vector(const Vector& source)
{
	_size = source._size;
	_capacity = source._capacity;
	_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		_elements[i] = source._elements[i];
	}
}

template<typename T>
inline my::Vector<T>::~Vector()
{
	delete[] _elements;
}

template<typename T>
inline void my::Vector<T>::push_back(const T& element)
{
	if (_size == _capacity)
		adjust_capacity(_capacity * 2);

	_elements[_size] = element;
	_size++;
}

template<typename T>
inline void my::Vector<T>::pop_back()
{
	if (_size > 0) {
		_size--;
	}
}

template<typename T>
inline void my::Vector<T>::insert(int index, const T& element)
{
	index = adjust_index(index);

	if (index == _size) {
		push_back(element);
		return;
	}
	
	if (_size == _capacity) {
		_capacity *= 2;
		T* new_elements = new T[_capacity];
		for (int i = 0; i < index; i++) {
			new_elements[i] = _elements[i];
		}
		new_elements[index] = element;
		for (int i = index; i < _size; i++) {
			new_elements[i + 1] = _elements[i];
		}
		delete[] _elements;
		_elements = new_elements;
		_size++;
		return;
	}

	for (int i = _size - 1; i >= index; i--) {
		_elements[i] = _elements[i - 1];
	}
	_elements[index] = element;
	_size++;
}

template<typename T>
inline void my::Vector<T>::erase(int index)
{
	index = adjust_index(index);

	_size--;
	for (int i = index; i < _size; i++) {
		_elements[i] = _elements[i + 1];
	}
}

template<typename T>
inline void my::Vector<T>::adjust_capacity()
{
	adjust_capacity(std::max(2, _size * 2));
}

template<typename T>
inline void my::Vector<T>::adjust_capacity(int capacity)
{
	if (capacity < _size)
		throw std::invalid_argument("Cannot decrease capacity lower than current size.");
	if (capacity < 1)
		throw std::invalid_argument("Cannot have a capacity less than 1.");

	_capacity = capacity;
	T* new_elements = new T[_capacity];
	for (int i = 0; i < _size; i++) {
		new_elements[i] = _elements[i];
	}
	delete[] _elements;
	_elements = new_elements;
}

template<typename T>
inline my::Vector<T>& my::Vector<T>::operator=(const my::Vector<T>& source)
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



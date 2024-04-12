#pragma once

namespace my {
	template <typename T>
	class Vector;
}

template <typename T>
class my::Vector
{
public:
	// Constructors and Destructor
	Vector(int initial_capacity = 10);
	Vector(int size, const T& inital_values);
	Vector(const Vector& source);
	~Vector();

	// Methods
	int size() const { return _size; };
	int capacity() const { return _capacity; };
	bool is_empty() const { return _size == 0; }
	void clear();
	void push_back(const T& element);
	void pop_back();
	void insert(int index, const T& element);
	void erase(int index);
	T& back() const;
	T& front() const;

	// Operators
	T& operator [](int index);
	const T& operator [](int index) const;
	Vector& operator =(const Vector& source);


private:
	// Fields
	int _size;
	int _capacity;
	T* _elements;
};

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

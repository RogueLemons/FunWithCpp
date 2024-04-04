#pragma once
#include <string>


class BigInt
{
public:
	// Constructor
	BigInt(const std::string& number_as_string);
	BigInt(int number = 0);

	// Methods
	std::string to_string() const;
	void add_to_this(const BigInt& big_int);
	BigInt add(const BigInt& big_int) const;
	void subtract_from_this(const BigInt& big_int);
	BigInt subtract(const BigInt& big_int) const;

private:
	// Fields
	std::string digits;
	bool is_positive;

	// Helper methods
	void set_sign(bool is_positive);
};


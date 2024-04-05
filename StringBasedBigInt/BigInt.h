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
	void multiply_this_by(const BigInt& big_int);
	BigInt multiply(const BigInt& big_int) const;

private:
	// Fields
	std::string digits;
	bool is_positive;

	// Helper methods
	void set_sign(bool is_positive);
	void unsigned_add_to_this(const BigInt& big_int);
	void unsigned_subtract_from_this(const BigInt& big_int);
	bool abs_is_bigger_than(const BigInt& big_int) const;
	void multiply_this_by_single_digit(int factor);
};


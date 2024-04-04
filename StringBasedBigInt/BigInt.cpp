#include "BigInt.h"
#include <stdexcept>

// Helper Functions
namespace {

	int as_int(char c) {
		return c - '0';
	}

	char as_char(int i) {
		return i + '0';
	}

}

BigInt::BigInt(const std::string& number_as_string)
{
	if (number_as_string.length() < 1)
		throw std::invalid_argument("May not provide empty string as argument.");
	is_positive = number_as_string[0] != '-';

	int first_digit = is_positive ? 0 : 1;
	for (int i = number_as_string.length() - 1; i >= first_digit; i--) {
		auto digit = number_as_string[i];
		if (!std::isdigit(digit))
			throw std::invalid_argument("May not provide non-digit characters in string.");
		digits.push_back(digit);
	}
}

BigInt::BigInt(int number) : BigInt(std::to_string(number))
{}

std::string BigInt::to_string() const
{
	std::string temp;
	if (!is_positive)
		temp.push_back('-');
	for (int i = digits.length() - 1; i >= 0; i--)
		temp.push_back(digits[i]);
	return temp;
}

void BigInt::add_to_this(const BigInt& big_int)
{
	if (this->is_positive != big_int.is_positive) {
		subtract_from_this(big_int);
		return;
	}

	const auto a_digits = this->digits;
	const int a_length = a_digits.length();
	const auto& b_digits = big_int.digits;
	const int b_length = b_digits.length();
	const int longest = std::max(a_length, b_length);

	digits = "";
	int holdover = 0;
	for (int i = 0; i < longest; i++) {
		int a_digit = i < a_length ? as_int(a_digits[i]) : 0;
		int b_digit = i < b_length ? as_int(b_digits[i]) : 0;
		int sum = a_digit + b_digit + holdover;

		int digit = sum % 10;
		holdover = (a_digit + b_digit) / 10;
		digits.push_back(as_char(digit));
	}
	if (holdover > 0)
		digits.push_back('1');
}

BigInt BigInt::add(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.add_to_this(big_int);
	return temp;
}

void BigInt::subtract_from_this(const BigInt& big_int)
{
	if (this->is_positive == big_int.is_positive) {
		add_to_this(big_int);
		return;
	}



}

BigInt BigInt::subtract(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.subtract_from_this(big_int);
	return temp;
}

void BigInt::set_sign(bool is_positive)
{
	this->is_positive = is_positive;
}


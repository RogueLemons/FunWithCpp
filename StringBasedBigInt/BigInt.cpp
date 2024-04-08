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
	if (number_as_string.length() > 1 && number_as_string[first_digit] == '0')
		throw std::invalid_argument("May not start number with the digit 0.");

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
	if (this->is_positive == big_int.is_positive)
		unsigned_add_to_this(big_int);
	else
		unsigned_subtract_from_this(big_int);
}

BigInt BigInt::add(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.add_to_this(big_int);
	return temp;
}

void BigInt::subtract_from_this(const BigInt& big_int)
{
	if (this->is_positive == big_int.is_positive)
		unsigned_subtract_from_this(big_int);
	else
		unsigned_add_to_this(big_int);
}

BigInt BigInt::subtract(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.subtract_from_this(big_int);
	return temp;
}

void BigInt::multiply_this_by(const BigInt& big_int)
{
	if (!big_int.is_positive)
		set_sign(!is_positive);

	BigInt product, digit_product;
	std::string power;
	for (const auto& digit : big_int.digits) {
		digit_product.digits = digits;
		digit_product.multiply_this_by_single_digit(as_int(digit));
		digit_product.digits += power;
		product.add_to_this(digit_product);
		power.push_back('0');
	}
	this->digits = product.digits;
}

BigInt BigInt::multiply(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.multiply_this_by(big_int);
	return temp;
}

void BigInt::set_sign(bool is_positive)
{
	this->is_positive = is_positive;
}

void BigInt::unsigned_add_to_this(const BigInt& big_int)
{
	const auto a_digits = this->digits;
	const auto& b_digits = big_int.digits;
	const int longest = std::max(a_digits.length(), b_digits.length());

	digits = "";
	int holdover = 0;
	for (int i = 0; i < longest; i++) {
		int a_digit = i < a_digits.length() ? as_int(a_digits[i]) : 0;
		int b_digit = i < b_digits.length() ? as_int(b_digits[i]) : 0;
		int sum = a_digit + b_digit + holdover;

		int digit = sum % 10;
		holdover = (a_digit + b_digit) / 10;
		digits.push_back(as_char(digit));
	}
	if (holdover > 0)
		digits.push_back('1');
}

void BigInt::unsigned_subtract_from_this(const BigInt& big_int)
{
	const auto is_biggest_abs = abs_is_bigger_than(big_int);
	const std::string temp = digits;
	const auto& a_digits = is_biggest_abs ? temp : big_int.digits;
	const auto& b_digits = is_biggest_abs ? big_int.digits : temp;

	digits = "";
	int steal = 0;
	for (int i = 0; i < a_digits.length(); i++) {
		int a_digit = as_int(a_digits[i]);
		int b_digit = i < b_digits.length() ? as_int(b_digits[i]) : 0;
		int diff = a_digit - b_digit - steal;

		if (diff >= 0) {
			digits.push_back(as_char(diff));
			steal = 0;
		}
		else {
			digits.push_back(as_char(10 + diff));
			steal = 1;
		}
	}

	if (!is_biggest_abs)
		set_sign(!is_positive);
}

bool BigInt::abs_is_bigger_than(const BigInt& big_int) const
{
	auto this_length = digits.length();
	auto that_length = big_int.digits.length();
	if (this_length != that_length)
		return this_length > that_length;

	for (int i = this_length; i >= 0; i--) {
		auto this_digit = as_int(digits[i]);
		auto that_digit = as_int(big_int.digits[i]);

		if (this_digit != that_digit)
			return this_digit > that_digit;
	}

	return false;
}

void BigInt::multiply_this_by_single_digit(int factor)
{
	if (factor == 0) {
		digits = "0";
		set_sign(true);
		return;
	}
	if (factor < 0) {
		set_sign(!is_positive);
		factor = abs(factor);
	}
	if (factor > 9) {
		throw std::invalid_argument("May not provide integer whose absolute value is larger than 9.");
	}

	BigInt product;
	std::string power;
	for (auto& digit : digits) {
		auto digit_product = std::to_string(as_int(digit) * factor) + power;
		product.add_to_this({ digit_product });
		power.push_back('0');
	}
	this->digits = product.digits;
}


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
	if (number_as_string == "")
		throw std::invalid_argument("May not provide empty string as argument.");

	is_positive = number_as_string[0] != '-';
	int first_digit = is_positive ? 0 : 1;
	if (number_as_string[first_digit] == '0' && number_as_string.length() > 1)
		throw std::invalid_argument("May not start number with the digit 0.");

	auto i = number_as_string.length();
	while (true) {
		i--;
		auto digit = number_as_string[i];
		if (!std::isdigit(digit))
			throw std::invalid_argument("May not provide non-digit characters in string.");
		digits.push_back(digit);

		if (i == first_digit)
			break;
	}
}

BigInt::BigInt(long long number) : BigInt(std::to_string(number))
{}

std::string BigInt::to_string() const
{
	std::string temp = digits;
	if (!is_positive)
		temp.push_back('-');
	std::reverse(temp.begin(), temp.end());
	return temp;
}

bool BigInt::equals(const BigInt& big_int) const
{
	if (this->is_positive != big_int.is_positive)
		return false;

	return abs_equals(big_int);
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
	if (big_int.digits == "0") {
		is_positive = true;
		digits = "0";
		return;
	}

	if (!big_int.is_positive)
		is_positive = !is_positive;

	BigInt product, digit_product;
	std::string power;
	for (const auto& digit : big_int.digits) {
		digit_product.digits = digits;
		digit_product.multiply_this_by_single_digit(as_int(digit));
		digit_product.digits = power + digit_product.digits;
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

void BigInt::divide_this_by(const BigInt& big_int)
{
	if (big_int.digits == "0")
		throw std::invalid_argument("May not divide by 0.");
	if (!abs_is_bigger_than(big_int)) {
		digits = "0";
		is_positive = true;
		return;
	}
	if (!big_int.is_positive)
		is_positive = !is_positive;

	auto power_diff = this->digits.length() - big_int.digits.length();
	auto test_factor_digits = (power_diff - 1 > 1) ? std::string(power_diff - 1, '0') : "1";
	test_factor_digits[0] = '1';
	auto test_factor = BigInt(test_factor_digits);

	BigInt test_product;
	do {
		test_factor.multiply_this_by_10();
		test_product = big_int.multiply(test_factor);
	} 
	while (abs_is_bigger_than(test_product));

	while (!abs_is_bigger_than(test_product)) {
		test_product.unsigned_subtract_from_this(big_int);
		test_factor.unsigned_subtract_from_this({ 1 });
	}

	this->digits = test_factor.digits;
}

BigInt BigInt::divide(const BigInt& big_int) const
{
	BigInt temp = *this;
	temp.divide_this_by(big_int);
	return temp;
}

void BigInt::unsigned_add_to_this(const BigInt& big_int)
{
	if (big_int.digits == "0")
		return;

	const auto a_digits(this->digits);
	const auto& b_digits = big_int.digits;
	const auto longest = std::max(a_digits.length(), b_digits.length());

	digits = "";
	int holdover = 0;
	for (size_t i = 0; i < longest; i++) {
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
	if (big_int.digits == "0")
		return;

	if (abs_equals(big_int)) {
		is_positive = true;
		digits = "0";
		return;
	}

	const auto is_biggest_abs = abs_is_bigger_than(big_int);
	const std::string temp = digits;
	const auto& a_digits = is_biggest_abs ? temp : big_int.digits;
	const auto& b_digits = is_biggest_abs ? big_int.digits : temp;

	digits = "";
	int steal = 0;
	for (size_t i = 0; i < a_digits.length(); i++) {
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

	while (digits.back() == '0')
		digits.pop_back();

	if (!is_biggest_abs)
		is_positive = !is_positive;
}

bool BigInt::abs_is_bigger_than(const BigInt& big_int) const
{
	auto this_length = digits.length();
	auto that_length = big_int.digits.length();
	if (this_length != that_length)
		return this_length > that_length;

	auto i = this_length;
	while (true) {
		i--;
		auto this_digit = as_int(digits[i]);
		auto that_digit = as_int(big_int.digits[i]);

		if (this_digit != that_digit)
			return this_digit > that_digit;
		if (i == 0)
			return false;
	}

	return false;
}

void BigInt::multiply_this_by_single_digit(int factor)
{
	if (factor == 0) {
		digits = "0";
		is_positive = true;
		return;
	}
	if (factor < 0) {
		is_positive = !is_positive;
		factor = abs(factor);
	}
	if (factor > 9) {
		throw std::invalid_argument("May not provide integer whose absolute value is larger than 9.");
	}

	BigInt product;
	std::string power;
	for (auto& digit : digits) {
		auto digit_product = as_int(digit) * factor;
		if (digit_product != 0)
			product.add_to_this({ std::to_string(digit_product) + power });
		power.push_back('0');
	}
	this->digits = product.digits;
}

void BigInt::multiply_this_by_10()
{
	digits = "0" + digits;
}

bool BigInt::abs_equals(const BigInt& big_int) const
{
	if (this->digits.length() != big_int.digits.length())
		return false;

	auto i = digits.length();
	while (true) {
		i--;
		if (this->digits[i] != big_int.digits[i])
			return false;
		if (i == 0)
			return true;
	}
}


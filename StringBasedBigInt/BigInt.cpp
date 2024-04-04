#include "BigInt.h"
#include <stdexcept>

BigInt::BigInt(const std::string& numberAsString) 
	: value(numberAsString)
{
	for (const auto& digit : value) {
		if (!std::isdigit(digit))
			throw std::invalid_argument("May not provide non-digit characters in string.");
	}
}

BigInt::BigInt(int number) 
	: value(std::to_string(number))
{}

const std::string& BigInt::to_string() const
{
	return value;
}

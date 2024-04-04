#pragma once
#include <string>


class BigInt
{
public:
	// Constructor
	BigInt(const std::string& numberAsString);
	BigInt(int number = 0);

	// Methods
	BigInt Add(BigInt bigInt) const;
	const std::string& to_string() const;

private:
	// Fields
	std::string value;

	// Helper methods

};


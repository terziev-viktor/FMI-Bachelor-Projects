#pragma once
#include <stdexcept>

class incompatible_people : public std::exception
{
public:
	const char * what() const override
	{
		return "Incompatible people. The only options are:\nfmi is compatible with fmi and tu\ntu is compatible with tu and unwe\nunwe is compatible with unwe and fmi";
	}
};

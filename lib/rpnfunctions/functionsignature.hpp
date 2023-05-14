#pragma once

#include "rpnfunctions/typedef.hpp"
#include <vector>

struct FunctionSignature {
	std::vector<RPNValueType> args;
	RPNValueType returnType;
	bool builtin;
	bool callable = true;
};
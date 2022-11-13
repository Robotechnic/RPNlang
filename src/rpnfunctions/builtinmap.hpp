#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"

#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"

class BuiltinRPNFunction;
namespace builtins {
	extern const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;
};
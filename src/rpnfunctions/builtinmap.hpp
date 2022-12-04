#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "modules/module.hpp"

class BuiltinRPNFunction;
namespace builtins {
	extern const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;
};
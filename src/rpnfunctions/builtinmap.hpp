#pragma once

#include <string>
#include <vector>
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"

#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"

namespace builtins {
	extern std::map<std::string, BuiltinRPNFunction> builtinFunctions;
};
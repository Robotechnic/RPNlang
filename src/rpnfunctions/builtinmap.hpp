#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "modules/module.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "rpnfunctions/typedef.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"

class BuiltinRPNFunction;
namespace builtins {
extern const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;
};
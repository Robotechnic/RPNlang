#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <variant>
#include "context/typedef.hpp"
#include "value/valuetypes.hpp"

class Value;
class ExpressionResult;
class Context;
class TextRange;

using RPNFunctionArgsValue = std::vector<Value *>;
using RPNFunctionResult = std::variant<ExpressionResult, Value *>;
using RPNFunctionArgs = std::vector<std::pair<std::string, RPNValueType>>;

using BuiltinRPNFunctionType = std::function<RPNFunctionResult (RPNFunctionArgsValue &, TextRange &, ContextPtr)>;

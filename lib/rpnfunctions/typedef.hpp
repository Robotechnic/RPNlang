#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include <variant>
#include "context/typedef.hpp"
#include "value/valuetypes.hpp"

class Value;
class ExpressionResult;
class Context;
class TextRange;

typedef std::vector<Value*> RPNFunctionArgs;
typedef std::pair<ExpressionResult, Value*> RPNFunctionResult;
typedef std::function<RPNFunctionResult(RPNFunctionArgs&, TextRange&, ContextPtr)> BuiltinRPNFunctionType;
typedef std::variant<std::string, ValueType> RPNFunctionValueType;
typedef std::vector<RPNFunctionValueType> RPNFunctionArgTypes;
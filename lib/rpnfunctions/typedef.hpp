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

typedef std::vector<Value*> RPNFunctionArgs;
typedef std::variant<ExpressionResult, Value*> RPNFunctionResult;
typedef std::function<RPNFunctionResult(RPNFunctionArgs&, TextRange&, ContextPtr)> BuiltinRPNFunctionType;
typedef std::variant<std::string, ValueType> RPNValueType;
typedef std::vector<RPNValueType> RPNFunctionArgTypes;
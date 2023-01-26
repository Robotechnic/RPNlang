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

typedef std::vector<Value*> RPNFunctionArgsValue;
typedef std::variant<ExpressionResult, Value*> RPNFunctionResult;
typedef std::vector<std::pair<std::string, RPNValueType>> RPNFunctionArgs;

typedef std::function<RPNFunctionResult(RPNFunctionArgsValue&, TextRange&, ContextPtr)> BuiltinRPNFunctionType;

#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include "context/typedef.hpp"

class Value;
class ExpressionResult;
class Context;
class TextRange;

typedef const std::vector<Value*> RPNFunctionArgs;
typedef std::tuple<ExpressionResult, Value*> RPNFunctionResult;
typedef std::function<RPNFunctionResult(RPNFunctionArgs&, TextRange&, ContextPtr)> BuiltinRPNFunctionType;
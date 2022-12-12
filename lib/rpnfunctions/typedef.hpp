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

typedef std::vector<Value*> RPNFunctionArgs;
typedef std::pair<ExpressionResult, Value*> RPNFunctionResult;
typedef std::function<RPNFunctionResult(const RPNFunctionArgs&, TextRange&, ContextPtr)> BuiltinRPNFunctionType;
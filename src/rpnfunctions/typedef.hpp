#pragma once

#include <vector>
#include <tuple>
#include <functional>

class Value;
class ExpressionResult;
class Context;

typedef std::vector<Value*> RPNFunctionArgs;
typedef std::tuple<ExpressionResult, Value*> RPNFunctionResult;
typedef std::function<RPNFunctionResult(RPNFunctionArgs&, Context*)> BuiltinRPNFunctionType;
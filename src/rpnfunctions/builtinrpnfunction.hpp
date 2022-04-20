#pragma once

#include <string>
#include <functional>
#include <tuple>
#include "value/value.hpp"
#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/rpnfunction.hpp"


template <class Signature>
class BuiltInRPNFunction : public RPNFunction {
	public:
		BuiltInRPNFunction(
			std::string name,
			std::vector<std::string> parameters,
			std::vector<ValueType> parameterTypes,
			ValueType returnType, 
			std::function<Signature> func
		);

		std::tuple<ExpressionResult, Value> call(
			std::vector<Value> args,
			std::map<std::string, Value> variables,
			std::map<std::string, RPNFunction> functions
		);
	
	private:
		std::function<Signature> func;
};
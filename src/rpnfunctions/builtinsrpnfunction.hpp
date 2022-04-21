#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include "rpnfunctions/rpnfunction.hpp"
#include "value/value.hpp"
#include "value/valuetype.hpp"

class BuiltinRPNFunction : public RPNFunction {
	public:
		BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			std::function<RPNFunctionResult(RPNFunctionArgs&)> function
		);

		RPNFunctionResult call(
			RPNFunctionArgs args,
			std::map<std::string, Value> variables,
			std::map<std::string, RPNFunction *> functions
		) const;

		static const std::map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		std::function<RPNFunctionResult(RPNFunctionArgs&)> function;
};
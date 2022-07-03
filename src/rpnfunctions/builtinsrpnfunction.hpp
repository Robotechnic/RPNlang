#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include "rpnfunctions/rpnfunction.hpp"
#include "modules/module.hpp"
#include "value/valuetype.hpp"
#include "value/value.hpp"

class BuiltinRPNFunction : public RPNFunction {
	public:
		BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			std::function<RPNFunctionResult(RPNFunctionArgs&, Context*)> function
		);
		~BuiltinRPNFunction();

		RPNFunctionResult call(
			RPNFunctionArgs args,
			Context *context
		) const;

		TextRange getRange() const;

		static const std::map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		std::function<RPNFunctionResult(RPNFunctionArgs&, Context*)> function;
};
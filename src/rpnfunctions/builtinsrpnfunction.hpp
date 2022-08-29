#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <cassert>
#include "rpnfunctions/rpnfunction.hpp"
#include "modules/module.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"
#include "rpnfunctions/typedef.hpp"
#include "tokens/regex.hpp"

class BuiltinRPNFunction : public RPNFunction {
	public:
		BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		);
		~BuiltinRPNFunction();

		RPNFunctionResult call(
			RPNFunctionArgs args,
			Context *context
		) const;

		TextRange getRange() const;

		static const std::map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		BuiltinRPNFunctionType function;
};
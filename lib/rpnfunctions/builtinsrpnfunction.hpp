#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <cassert>
#include <unordered_map>
#include "rpnfunctions/rpnfunction.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "rpnfunctions/typedef.hpp"

class BuiltinRPNFunction : public RPNFunction {
	public:
		BuiltinRPNFunction();
		BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		);
		~BuiltinRPNFunction();

		RPNFunctionResult call(
			const RPNFunctionArgs &args,
			const TextRange &range,
			ContextPtr context
		) const;

		static const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		BuiltinRPNFunctionType function;
};
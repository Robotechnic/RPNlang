#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <cassert>
#include <unordered_map>
#include "rpnfunctions/rpnfunction.hpp"
#include "modules/module.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "rpnfunctions/typedef.hpp"

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
			const RPNFunctionArgs &args,
			const TextRange &range,
			Context *context
		) const;

		static const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		BuiltinRPNFunctionType function;
};
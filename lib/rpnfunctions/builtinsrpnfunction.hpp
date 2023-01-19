#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>
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
			std::string_view name,
			RPNFunctionArgs arguments,
			RPNValueType returnType,
			BuiltinRPNFunctionType function
		);
		~BuiltinRPNFunction();

		RPNFunctionResult call(
			RPNFunctionArgsValue &args,
			const TextRange &range,
			ContextPtr context
		) const override;

		static const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;

	private:
		BuiltinRPNFunctionType function;
};
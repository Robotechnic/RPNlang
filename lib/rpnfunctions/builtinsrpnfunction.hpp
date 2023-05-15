#pragma once

#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/typedef.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include <cassert>
#include <functional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

class BuiltinRPNFunction : public RPNFunction {
  public:
	BuiltinRPNFunction();
	BuiltinRPNFunction(std::string_view name, RPNFunctionArgs arguments, RPNValueType returnType,
					   BuiltinRPNFunctionType function);
	~BuiltinRPNFunction() override = default;

	RPNFunctionResult call(RPNFunctionArgsValue &args, const TextRange &range,
						   ContextPtr context) const override;

	static const std::unordered_map<std::string, BuiltinRPNFunction> builtinFunctions;

  private:
	BuiltinRPNFunctionType function;
};
#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			std::function<RPNFunctionResult(RPNFunctionArgs&)> function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {}

RPNFunctionResult BuiltinRPNFunction::call(
	RPNFunctionArgs args,
	std::map<std::string, Value> variables,
	std::map<std::string, RPNFunction *> functions
) const {
	ExpressionResult result = this->checkArgs(args);
	if (result.error()) return std::make_tuple(result, Value());
	return this->function(args);
}


const std::map<std::string, BuiltinRPNFunction> BuiltinRPNFunction::builtinFunctions = {
	{"cos",BuiltinRPNFunction(
		"cos",
		std::vector<std::string>(1, "x"),
		std::vector<ValueType>(1, ValueType::FLOAT),
		ValueType::FLOAT,
		[](RPNFunctionArgs& args) -> RPNFunctionResult {
			return std::make_tuple(ExpressionResult(), Value(std::cos(args[0].getFloatValue()), 0, 0));		
		})
	}
};
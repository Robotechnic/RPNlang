#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {}

BuiltinRPNFunction::~BuiltinRPNFunction() {}

RPNFunctionResult BuiltinRPNFunction::call(
	RPNFunctionArgs args,
	Context *context
) const {
	context->setChild(new Context(this->name, "<builtin>", context, CONTEXT_TYPE_FUNCTION));

	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) return std::make_tuple(result, nullptr);
	RPNFunctionResult functionResult = this->function(args, context->getChild());
	
	assert(
		std::get<1>(functionResult) != nullptr &&
		"BuiltinRPNFunction::call: functionResult.second is nullptr"
	);

	return functionResult;
}
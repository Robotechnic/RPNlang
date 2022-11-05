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

	RPNFunctionResult result = std::make_tuple(this->checkArgs(args, context), None::empty());
	if (std::get<0>(result).error()) return result;
	delete std::get<1>(result);
	result = this->function(args, context->getChild());
	assert(
		std::get<1>(result) != nullptr &&
		"BuiltinRPNFunction::call: result.second is nullptr"
	);

	return result;
}
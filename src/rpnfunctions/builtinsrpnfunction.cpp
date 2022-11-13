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
	const RPNFunctionArgs &args,
	const TextRange &range,
	Context *context
) const {
	context->setChild(new Context(this->name, "<builtin>", context, CONTEXT_TYPE_FUNCTION));

	RPNFunctionResult result = std::make_tuple(this->checkArgs(args, context), None::empty());
	if (std::get<0>(result).error()) return result;
	delete std::get<1>(result);
	TextRange functionRange = range;
	if (args.size() > 0) {
		functionRange.merge(args[0]->getRange());
	}
	result = this->function(args, functionRange, context->getChild());
	assert(
		std::get<1>(result) != nullptr &&
		"BuiltinRPNFunction::call: result.second is nullptr"
	);

	return result;
}
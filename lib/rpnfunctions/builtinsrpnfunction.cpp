#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction() : RPNFunction("",{},{},NONE), function(nullptr) {}

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {
	if (this->function == nullptr) {
		throw std::invalid_argument("BuiltinRPNFunction::BuiltinRPNFunction: function is nullptr");
	}
	if (argsName.size() != argumentsTypes.size()) {
		throw std::invalid_argument("BuiltinRPNFunction::BuiltinRPNFunction: argsName.size() != argumentsTypes.size()");
	}
}

BuiltinRPNFunction::~BuiltinRPNFunction() {}

RPNFunctionResult BuiltinRPNFunction::call(
	const RPNFunctionArgs &args,
	const TextRange &range,
	ContextPtr context
) const {
	ContextPtr functionContext = std::make_shared<Context>(this->name, "<builtin>", context, CONTEXT_TYPE_FUNCTION);

	RPNFunctionResult result = std::make_tuple(this->checkArgs(args, context), None::empty());
	if (std::get<0>(result).error()) return result;
	delete std::get<1>(result);
	TextRange functionRange = range;
	if (args.size() > 0) {
		functionRange.merge(args[0]->getRange());
	}

	RPNFunctionArgs converted;
	for (size_t i = 0; i < args.size(); i++) {
		converted.push_back(args.at(i)->to(this->argsTypes.at(i)));
	}
	result = this->function(converted, functionRange, functionContext);
	for (Value *val : converted)
		delete val;
	assert(
		std::get<1>(result) != nullptr &&
		"BuiltinRPNFunction::call: result.second is nullptr"
	);

	return result;
}
#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction() : RPNFunction("",{},{},NONE), function(nullptr) {}

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string_view name,
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
	RPNFunctionArgs &args,
	const TextRange &range,
	ContextPtr context
) const {
	ContextPtr functionContext = std::make_shared<Context>(this->name, "<builtin>", context, CONTEXT_TYPE_FUNCTION);

	RPNFunctionResult result = std::make_pair(this->checkTypes(args, context), None::empty());
	if (result.first.error()) return result;
	TextRange functionRange = range;
	if (args.size() > 0) {
		functionRange.merge(args[0]->getRange());
	}

	RPNFunctionArgs converted;
	for (size_t i = 0; i < args.size(); i++) {
		if (this->argsTypes.at(i) == ANY || args.at(i)->getType() == this->argsTypes.at(i))
			converted.push_back(args.at(i));
		else
			converted.push_back(args.at(i)->to(this->argsTypes.at(i)));
	}
	result = this->function(converted, functionRange, functionContext);
	for (size_t i = 0; i < args.size(); i++)
		if (this->argsTypes.at(i) != ANY && args.at(i)->getType() != this->argsTypes.at(i)) 
			delete converted.at(i);
	
	assert(
		result.second != nullptr &&
		"BuiltinRPNFunction::call: result.second is nullptr"
	);

	return result;
}
#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction() : RPNFunction("",{},{},NONE), function(nullptr) {}

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string_view name,
			std::vector<std::string> argsName,
			RPNFunctionArgTypes argumentsTypes,
			RPNFunctionValueType returnType,
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
	ContextPtr functionContext = std::make_shared<Context>(this->name, "<builtin>", context, CONTEXT_TYPE_BUILTIN_FUNCTION);

	RPNFunctionResult result = std::make_pair(this->checkTypes(args, context), None::empty());
	if (result.first.error()) return result;
	TextRange functionRange = range;
	if (args.size() > 0) {
		functionRange.merge(args[0]->getRange());
	}

	RPNFunctionArgs converted;
	for (size_t i = 0; i < args.size(); i++) {
		if (this->argsTypes.at(i).index() == 0) {
			converted.push_back(args.at(i));
			continue;
		}
		ValueType type = std::get<ValueType>(this->argsTypes.at(i));
		if (type == ANY || args.at(i)->getType() == type)
			converted.push_back(args.at(i));
		else
			converted.push_back(args.at(i)->to(type));
	}
	result = this->function(converted, functionRange, functionContext);
	for (size_t i = 0; i < args.size(); i++) {
		if (this->argsTypes.at(i).index() == 0) continue;
		ValueType type = std::get<ValueType>(this->argsTypes.at(i));
		if (type != ANY && args.at(i)->getType() != type) 
			delete converted.at(i);
	}
	
	assert(
		result.second != nullptr &&
		"BuiltinRPNFunction::call: result.second is nullptr"
	);

	if (result.first.error()) return result;

	if (this->returnType.index() == 0) {
		if (result.second->getType() != STRUCT)
			throw std::runtime_error("BuiltinRPNFunction::call: result.second->getType() != STRUCT");
		if (std::get<std::string>(this->returnType) != static_cast<Struct*>(result.second)->getStructName())
			throw std::runtime_error("BuiltinRPNFunction::call: std::get<std::string>(this->returnType) != result.second->getStructName()");
	} else {
		if (result.second->getType() != std::get<ValueType>(this->returnType))
			throw std::runtime_error("BuiltinRPNFunction::call: result.second->getType() != std::get<ValueType>(this->returnType)");
	}

	return result;
}
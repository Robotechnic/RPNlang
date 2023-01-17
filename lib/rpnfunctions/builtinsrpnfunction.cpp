#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction() : RPNFunction("",{},{},NONE), function(nullptr) {}

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string_view name,
			std::vector<std::string> argsName,
			RPNFunctionArgTypes argumentsTypes,
			RPNValueType returnType,
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

	ExpressionResult result = this->checkTypes(args, context);
	if (result.error()) return result;

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
	RPNFunctionResult callResult = this->function(converted, functionRange, functionContext);
	for (size_t i = 0; i < args.size(); i++) {
		if (this->argsTypes.at(i).index() == 0) continue;
		ValueType type = std::get<ValueType>(this->argsTypes.at(i));
		if (type != ANY && args.at(i)->getType() != type) 
			delete converted.at(i);
	}
	

	if (result = *std::get_if<ExpressionResult>(&callResult); result.error()) return callResult;

	if (this->returnType.index() == 0) {
		if (std::get<Value*>(callResult)->getType() != STRUCT)
			throw std::runtime_error("BuiltinRPNFunction::call: result.second->getType() != STRUCT");
		if (std::get<std::string>(this->returnType) != static_cast<Struct*>(std::get<Value*>(callResult))->getStructName())
			throw std::runtime_error("BuiltinRPNFunction::call: std::get<std::string>(this->returnType) != result.second->getStructName()");
	} else {
		if (!Value::isCastableTo(std::get<Value*>(callResult)->getType(), std::get<ValueType>(this->returnType)))
			throw std::runtime_error("BuiltinRPNFunction::call: result.second->getType() != std::get<ValueType>(this->returnType)");
	}

	return callResult;
}
#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction() : RPNFunction("", {}, NONE), function(nullptr) {}

BuiltinRPNFunction::BuiltinRPNFunction(std::string_view name, RPNFunctionArgs arguments,
									   RPNValueType returnType, BuiltinRPNFunctionType function)
	: RPNFunction(name, arguments, returnType), function(function) {
	if (this->function == nullptr) {
		throw std::invalid_argument("BuiltinRPNFunction::BuiltinRPNFunction: function is nullptr");
	}
}

RPNFunctionResult BuiltinRPNFunction::call(RPNFunctionArgsValue &args, const TextRange &range,
										   ContextPtr context) const {
	ContextPtr functionContext =
		std::make_shared<Context>(this->name, "<builtin>", context, CONTEXT_TYPE_BUILTIN_FUNCTION);

	TextRange functionRange = range;
	if (args.size() > 0) {
		functionRange.merge(args[0]->getRange());
	}

	RPNFunctionArgsValue converted;
	for (size_t i = 0; i < args.size(); i++) {
		if (this->arguments.at(i).second.index() == 0) {
			converted.push_back(args.at(i));
			continue;
		}
		ValueType type = std::get<ValueType>(this->arguments.at(i).second.getType());
		if (type == ANY || args.at(i)->getType() == type)
			converted.push_back(args.at(i));
		else
			converted.push_back(args.at(i)->to(type, Value::PARENT_FUNCTION));
	}
	RPNFunctionResult callResult = this->function(converted, functionRange, functionContext);
	for (size_t i = 0; i < args.size(); i++) {
		if (this->arguments.at(i).second.index() == 0) {
			continue;
		}
		ValueType type = std::get<ValueType>(this->arguments.at(i).second.getType());
		if (type != ANY && args.at(i)->getType() != type)
			Value::deleteValue(&converted.at(i), Value::PARENT_FUNCTION);
	}

	if (auto *callExpressionResult = std::get_if<ExpressionResult>(&callResult);
		callExpressionResult && callExpressionResult->error()) {
		return callResult;
	}

	if (this->returnType.index() == 0) {
		if (std::get<Value *>(callResult)->getType() != STRUCT)
			throw std::runtime_error(
				"BuiltinRPNFunction::call: result.second->getType() != STRUCT");
		if (std::get<std::string>(this->returnType.getType()) !=
			static_cast<Struct *>(std::get<Value *>(callResult))->getStructName())
			throw std::runtime_error(
				"BuiltinRPNFunction::call: std::get<std::string>(this->returnType) != "
				"result.second->getStructName()");
	} else {
		if (!RPNValueType::isCastableTo(std::get<Value *>(callResult)->getType(),
										std::get<ValueType>(this->returnType.getType())))
			throw std::runtime_error("BuiltinRPNFunction::call: result.second->getType() != "
									 "std::get<ValueType>(this->returnType.getType())");
	}

	return callResult;
}

FunctionSignature BuiltinRPNFunction::getSignature() const {
	FunctionSignature signature = RPNFunction::getSignature();
	signature.builtin = true;
	return signature;
}
#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string name,
	std::vector<std::string> argsName,
	std::vector<ValueType> parameterTypes,
	ValueType returnType
):
	name(name),
	argsName(argsName),
	parameterTypes(parameterTypes),
	returnType(returnType)
{}

RPNFunctionResult RPNFunction::call(
	RPNFunctionArgs args,
	std::map<std::string, Value> variables
) const {
	TextRange range(0, 0, 0);
	if (args.size() != 0) {
		range = args[0].getRange();
		if (args.size() > 1) {
			range.columnEnd = args[args.size() - 1].getRange().columnEnd;
		}
	}

	return std::make_tuple(ExpressionResult("Function is not callable", range), Value());
}

ExpressionResult RPNFunction::checkArgs(const RPNFunctionArgs &args) const{
	ExpressionResult result = this->checkTypes(args);
	if (result.error()) return result;
	if (args.size() != this->argsName.size()) {
		TextRange range(0, 0, 0);
		if (args.size() != 0) {
			range = args[0].getRange();
			if (args.size() > 1) {
				range.columnEnd = args[args.size() - 1].getRange().columnEnd;
			}
		}

		return ExpressionResult(
			"Function call arguments count mismatch," + 
			std::to_string(this->argsName.size()) + 
			" arguents expected but got " + 
			std::to_string(args.size()) + 
			" arguments",
			range
		);
	}

	return ExpressionResult();
}

ExpressionResult RPNFunction::checkTypes(const RPNFunctionArgs &args) const{
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i].getType() != this->parameterTypes[i]) {
			if (!args[i].isCastableTo(this->parameterTypes[i])) {
				return ExpressionResult(
					"Function call argument type mismatch, expected " + 
					Value::stringType(this->parameterTypes[i]) + 
					" but got " + Value::stringType(args[i].getType()),
					args[i].getRange()
				);
			}
		}
	}
	return ExpressionResult();
}

int RPNFunction::getArgumentsCount() const {
	return this->argsName.size();
}

std::string RPNFunction::getName() const {
	return this->name;
}

TextRange RPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}
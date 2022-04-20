#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string name,
	std::vector<std::string> parameters,
	std::vector<ValueType> parameterTypes,
	ValueType returnType, 
	std::string body
):
	name(name),
	parameters(parameters),
	parameterTypes(parameterTypes),
	returnType(returnType),
	body(body)
{}

ExpressionResult RPNFunction::tokenize() {
	return Token::tokenize(0, this->body, this->tokens);
}

ExpressionResult RPNFunction::checkArgs(const std::vector<Value> &args) {
	ExpressionResult result = this->checkTypes(args);
	if (result.error()) return result;
	if (args.size() != this->parameters.size()) {
		TextRange range(0, 0, 0);
		if (args.size() != 0) {
			range = args[0].getRange();
			if (args.size() > 1) {
				range.columnEnd = args[args.size() - 1].getRange().columnEnd;
			}
		}

		return ExpressionResult(
			"Function call arguments count mismatch," + std::to_string(this->parameters.size()) + " arguents expected but got " + std::to_string(args.size()) + " arguments",
			TextRange(0,0,0)
		);
	}

	return ExpressionResult();
}

ExpressionResult RPNFunction::checkTypes(const std::vector<Value> &args) {
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i].getType() != this->parameterTypes[i]) {
			return ExpressionResult(
				"Function call argument type mismatch (got " + Value::stringType(args[i].getType()) + " but expected " + Value::stringType(this->parameterTypes[i]) + ")",
				args[i].getRange()
			);
		}
	}
	return ExpressionResult();
}

void RPNFunction::addParameters(const std::vector<Value> &args, std::map<std::string, Value> &variables) {
	for (size_t i = 0; i < args.size(); i++) {
		variables[this->parameters[i]] = args[i];
	}
}

std::tuple<ExpressionResult, Value> RPNFunction::call(
	std::vector<Value> args,
	std::map<std::string, Value> variables,
	std::map<std::string, RPNFunction> functions
) {
	ExpressionResult result = this->checkArgs(args);
	if (result.error()) return std::make_tuple(result, Value());

	this->addParameters(args, variables);

	Interpreter interpreter(variables, functions);
	result = interpreter.interpret(this->tokens, 0);

	return std::make_tuple(result, interpreter.getLastValue());;
}
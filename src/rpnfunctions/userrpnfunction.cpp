#include "rpnfunctions/userrpnfunction.hpp"

UserRPNFunction::UserRPNFunction(
	std::string name,
	std::vector<std::string> argsName,
	std::vector<ValueType> parameterTypes,
	ValueType returnType, 
	std::queue<Token> body
):
	RPNFunction(name, argsName, parameterTypes, returnType),
	body(body)
{}

/**
 * @brief add a provided argument to the variables in context
 * 
 * @param args the arguments provided by the user
 * @param variables the variables in context
 */
void UserRPNFunction::addParameters(const RPNFunctionArgs &args, Context &context) const {
	for (size_t i = 0; i < args.size(); i++) {
		context.setValue(this->argsName[i], args[i]);
	}
}

RPNFunctionResult UserRPNFunction::call(
	RPNFunctionArgs args,
	const Context &context
) const {
	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) return std::make_tuple(result, Value());

	Context functionContext(this->name, &context, CONTEXT_TYPE_FUNCTION);

	this->addParameters(args, functionContext);

	// just for testin purposes, will be removed when return keyword and return type will be implemented
	Interpreter interpreter(functionContext);
	result = interpreter.interpret(this->body, 0);

	return std::make_tuple(result, interpreter.getLastValue());;
}

TextRange UserRPNFunction::getRange() const {
	TextRange result = this->body.front().getRange();
	if (this->body.size() > 1) {
		result.merge(this->body.back().getRange());
	}
	return result;
}
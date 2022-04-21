#include "rpnfunctions/userrpnfunction.hpp"

UserRPNFunction::UserRPNFunction(
	std::string name,
	std::vector<std::string> argsName,
	std::vector<ValueType> parameterTypes,
	ValueType returnType, 
	std::vector<Token> body
):
	RPNFunction(name, argsName, parameterTypes, returnType),
	body(body)
{}

void UserRPNFunction::addParameters(const RPNFunctionArgs &args, std::map<std::string, Value> &variables) const {
	for (size_t i = 0; i < args.size(); i++) {
		variables[this->argsName[i]] = args[i];
	}
}

RPNFunctionResult UserRPNFunction::call(
	RPNFunctionArgs args,
	std::map<std::string, Value> variables,
	std::map<std::string, RPNFunction*> functions
) const {
	ExpressionResult result = this->checkArgs(args);
	if (result.error()) return std::make_tuple(result, Value());

	this->addParameters(args, variables);

	Interpreter interpreter(variables, functions);
	result = interpreter.interpret(this->body, 0);

	return std::make_tuple(result, interpreter.getLastValue());;
}
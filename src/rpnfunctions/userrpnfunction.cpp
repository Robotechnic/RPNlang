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
{
	std::cout<<"Creating UserRPNFunction: "<<this->name<<std::endl;
}

UserRPNFunction::~UserRPNFunction() {
	std::cout<<"Deleting UserRPNFunction: "<<this->name<<std::endl;
}

/**
 * @brief add a provided argument to the variables in context
 * 
 * @param args the arguments provided by the user
 * @param variables the variables in context
 */
void UserRPNFunction::addParameters(const RPNFunctionArgs &args, Context *context) const {
	for (size_t i = 0; i < args.size(); i++) {
		context->setValue(this->argsName[i], args[i]);
	}
}

RPNFunctionResult UserRPNFunction::call(
	RPNFunctionArgs args,
	Context *context
) const {
	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) return std::make_tuple(result, Value());

	context->setChild(new Context(this->name, context, CONTEXT_TYPE_FUNCTION));

	this->addParameters(args, context->getChild());

	// just for testing purposes, will be removed when return keyword and return type will be implemented
	Interpreter interpreter(context->getChild());
	result = interpreter.interpret(this->body);

	return std::make_tuple(result, interpreter.getLastValue());;
}

TextRange UserRPNFunction::getRange() const {
	TextRange result = this->body.front().getRange();
	if (this->body.size() > 1) {
		result.merge(this->body.back().getRange());
	}
	return result;
}
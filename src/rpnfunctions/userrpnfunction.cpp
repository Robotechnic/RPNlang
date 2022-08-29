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

UserRPNFunction::~UserRPNFunction() {}

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
	if (result.error()) return std::make_tuple(result, nullptr);

	context->setChild(new Context(this->name, context, CONTEXT_TYPE_FUNCTION));

	this->addParameters(args, context->getChild());

	Interpreter interpreter(context->getChild());
	result = interpreter.interpret(this->body);

	if (result.error()) return std::make_tuple(result, nullptr);

	//check the return type	
	Value *returnValue = interpreter.getReturnValue();

	if (returnValue->getType() != this->returnType) {
		return std::make_tuple(
			ExpressionResult(
				returnValue->getType() == NONE ? 
					"Function " + this->name + " does not return any value" :
					"Function " + this->name + " expected a return value of type " + Value::stringType(this->returnType) + ", but no return value was found",
				this->body.back().getRange(),
				context
			),
			nullptr
		);
	}

	if (!returnValue->isCastableTo(this->returnType)) {
		return std::make_tuple(
			ExpressionResult(
				"Return type must be " + Value::stringType(this->returnType) + " but got " + Value::stringType(returnValue->getType()),
				returnValue->getRange(),
				context
			),
			nullptr
		);
	}

	return std::make_tuple(ExpressionResult(), returnValue->to(this->returnType));
}

TextRange UserRPNFunction::getRange() const {
	TextRange result = this->body.front().getRange();
	if (this->body.size() > 1) {
		result.merge(this->body.back().getRange());
	}
	return result;
}
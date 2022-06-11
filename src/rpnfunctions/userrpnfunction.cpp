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
	if (result.error()) return std::make_tuple(result, Value());

	context->setChild(new Context(this->name, context, CONTEXT_TYPE_FUNCTION));

	this->addParameters(args, context->getChild());

	Interpreter interpreter(context->getChild());
	result = interpreter.interpret(this->body);

	if (result.error()) return std::make_tuple(result, Value());

	//check the return type	
	Value returnValue = interpreter.getLastValue();
	if (returnValue.isCastableTo(this->returnType)) {
		return std::make_tuple(ExpressionResult(), returnValue.to(this->returnType));
	}
	
	TextRange returnValueRange = returnValue.getRange();
	std::string message = "Return type must be " + Value::stringType(this->returnType) + " but got " + Value::stringType(returnValue.getType());
	if (returnValueRange.isEmpty()) {
		returnValueRange = this->body.back().getRange();
		if (this->returnType == NONE) {
			message = "Unexpected return value, for function " + this->name;
		} else {
			message = "Function " + this->name + " expected a return value of type " + Value::stringType(this->returnType) + ", but no return value was found";
		}
	}
	
	return std::make_tuple(
		ExpressionResult(
			message,
			returnValueRange,
			context
		),
		Value()
	);
}

TextRange UserRPNFunction::getRange() const {
	TextRange result = this->body.front().getRange();
	if (this->body.size() > 1) {
		result.merge(this->body.back().getRange());
	}
	return result;
}
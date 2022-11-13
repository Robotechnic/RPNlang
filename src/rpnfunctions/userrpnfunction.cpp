#include "rpnfunctions/userrpnfunction.hpp"

UserRPNFunction::UserRPNFunction() : RPNFunction("",{},{},NONE), body(nullptr) {}
UserRPNFunction::UserRPNFunction(const UserRPNFunction &other) :
	RPNFunction(other.name, other.argsName, other.argsTypes, other.returnType),
	body(other.body)
{}

UserRPNFunction::UserRPNFunction(
	const std::string &name,
	const std::vector<std::string> &argsName,
	const std::vector<ValueType> &argsTypes,
	const ValueType &returnType,
	CodeBlock *body
):
	RPNFunction(name, argsName, argsTypes, returnType),
	body(body) {}

UserRPNFunction::~UserRPNFunction() {
	if (this->body != nullptr) {
		delete this->body;
	}
}

/**
 * @brief add a provided argument to the variables in context
 * 
 * @param args the arguments provided by the user
 * @param variables the variables in context
 */
void UserRPNFunction::addParameters(const RPNFunctionArgs &args, Context *context) const {
	for (size_t i = 0; i < args.size(); i++) {
		context->setValue(this->argsName[i], args[i]->to(this->argsTypes[i], false));
	}
}

RPNFunctionResult UserRPNFunction::call(
	const RPNFunctionArgs &args,
	const TextRange &range,
	Context *context
) const {
	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) 
		return std::make_tuple(result, None::empty());

	context->setChild(new Context(this->name, "", context, CONTEXT_TYPE_FUNCTION));
	this->addParameters(args, context->getChild());

	Interpreter interpreter(context->getChild());
	result = interpreter.interpret(this->body->getBlocks());

	if (result.error()) 
		return std::make_tuple(result, None::empty());

	//check the return type	
	Value *returnValue = interpreter.getReturnValue();

	if (returnValue->getType() != this->returnType) {
		return std::make_tuple(
			ExpressionResult(
				returnValue->getType() == NONE ? 
					"Function " + this->name + " does not return any value" :
					"Function " + this->name + " expected a return value of type " + Value::stringType(this->returnType) + ", but no return value was found",
				this->body->lastRange(),
				context
			),
			None::empty()
		);
	}

	if (!returnValue->isCastableTo(this->returnType)) {
		return std::make_tuple(
			ExpressionResult(
				"Return type must be " + Value::stringType(this->returnType) + " but got " + Value::stringType(returnValue->getType()),
				returnValue->getRange(),
				context
			),
			None::empty()
		);
	}

	return std::make_tuple(ExpressionResult(), returnValue->to(this->returnType));
}


std::shared_ptr<UserRPNFunction> UserRPNFunction::addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argsTypes,
			ValueType returnType, 
			CodeBlock *body
		) 
{
	userFunctions[name] = std::make_shared<UserRPNFunction>(name, argsName, argsTypes, returnType, body);
	return userFunctions.at(name);
}

std::shared_ptr<UserRPNFunction> UserRPNFunction::getFunction(std::string name) {
	if (userFunctions.find(name) == userFunctions.end())
		return nullptr;
	return userFunctions[name];
}

TextRange UserRPNFunction::getRange() const {
	return this->body->getRange();
}

std::map<std::string, std::shared_ptr<UserRPNFunction>> UserRPNFunction::userFunctions;
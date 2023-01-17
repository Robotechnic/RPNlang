#include "rpnfunctions/userrpnfunction.hpp"

UserRPNFunction::UserRPNFunction() : RPNFunction("",{},{},NONE), body(nullptr) {}
UserRPNFunction::UserRPNFunction(const UserRPNFunction &other) :
	RPNFunction(other.name, other.argsName, other.argsTypes, other.returnType),
	body(other.body)
{}

UserRPNFunction::UserRPNFunction(
	const std::string &name,
	const std::vector<std::string> &argsName,
	const RPNFunctionArgTypes &argsTypes,
	const RPNValueType &returnType,
	CodeBlock *body
):
	RPNFunction(name, argsName, argsTypes, returnType),
	body(body) {}

UserRPNFunction::~UserRPNFunction() {
	if (this->body != nullptr) {
		delete this->body;
	}
}

RPNFunctionResult UserRPNFunction::call(
	RPNFunctionArgs &args,
	const TextRange &range,
	ContextPtr context
) const {
	ExpressionResult result = this->checkTypes(args, context);
	if (result.error()) return result;

	ContextPtr functionContext = std::make_shared<Context>(this->name, "", context, CONTEXT_TYPE_FUNCTION);
	for (size_t i = 0; i < args.size(); i++) {
		if (this->argsTypes[i].index() == 0) {
			functionContext->setValue(
				this->argsName[i],
				args[i]->copy(Value::CONTEXT_VARIABLE)
			);
		} else {
			functionContext->setValue(
				this->argsName[i], 
				args[i]->to(std::get<ValueType>(this->argsTypes[i]), Value::CONTEXT_VARIABLE)
			);
		}
	}

	Interpreter interpreter(functionContext);
	result = interpreter.interpret(this->body->getBlocks());

	if (result.error()) return result;


	//check the return type	
	if (!result.returnValue()) {
		if (this->returnType.index() == 0 || (this->returnType.index() == 1 && std::get<ValueType>(this->returnType) != NONE))
			return ExpressionResult(
				"Function " + this->name + " expected a return value of type " + 
				(this->returnType.index() == 0 ? std::get<std::string>(this->returnType) : Value::stringType(std::get<ValueType>(this->returnType))) + 
				", but no return value was found",
				this->body->lastRange(),
				context
			);
		
		return None::empty();
	}

	Value *returnValue = interpreter.getLastValue();
	if (this->returnType.index() == 0) {
		if (returnValue->getType() != STRUCT)
			return ExpressionResult(
				"Return type must be struct of type " + 
				std::get<std::string>(this->returnType) + 
				" but got " + Value::stringType(returnValue->getType()),
				returnValue->getRange(),
				context
			);
		
		if (std::get<std::string>(this->returnType) != static_cast<Struct*>(returnValue)->getStructName())
			return ExpressionResult(
				"Return type must be struct of type " + 
				std::get<std::string>(this->returnType) + 
				" but got " + std::string(static_cast<Struct*>(returnValue)->getStructName()),
				returnValue->getRange(),
				context
			);

		return returnValue->copy();
	}
	
	if (std::get<ValueType>(this->returnType) == NONE) {
		return ExpressionResult(
			"Function " + this->name + " does not return any value",
			this->body->lastRange(),
			context
		);
	}

	if (!Value::isCastableTo(returnValue->getType(), std::get<ValueType>(this->returnType))) {
		return ExpressionResult(
			"Return type must be " + Value::stringType(std::get<ValueType>(this->returnType)) + " but got " + Value::stringType(returnValue->getType()),
			returnValue->getRange(),
			context
		);
	}

	return returnValue->to(std::get<ValueType>(this->returnType));
}


std::shared_ptr<UserRPNFunction> UserRPNFunction::addFunction(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const RPNFunctionArgTypes &argsTypes,
			RPNValueType returnType, 
			CodeBlock *body
		) 
{
	userFunctions[name] = std::make_shared<UserRPNFunction>(name, argsName, argsTypes, returnType, body);
	return userFunctions.at(name);
}

std::shared_ptr<UserRPNFunction> UserRPNFunction::getFunction(const std::string &name) {
	if (userFunctions.find(name) == userFunctions.end())
		return nullptr;
	return userFunctions[name];
}


CodeBlock *UserRPNFunction::getBody() const {
	return this->body;
}

TextRange UserRPNFunction::getRange() const {
	return this->body->getRange();
}

std::unordered_map<std::string, std::shared_ptr<UserRPNFunction>> UserRPNFunction::userFunctions;
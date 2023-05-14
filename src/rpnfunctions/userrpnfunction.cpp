#include "rpnfunctions/userrpnfunction.hpp"

UserRPNFunction::UserRPNFunction() : RPNFunction("", {}, NONE), body(nullptr) {}
UserRPNFunction::UserRPNFunction(const UserRPNFunction &other)
	: RPNFunction(other.name, other.arguments, other.returnType), body(other.body) {}

UserRPNFunction::UserRPNFunction(const std::string &name, const RPNFunctionArgs &arguments,
								 const RPNValueType &returnType, CodeBlock *body)
	: RPNFunction(name, arguments, returnType), body(body) {}

UserRPNFunction::~UserRPNFunction() {
	if (this->body != nullptr) {
		delete this->body;
	}
}

RPNFunctionResult UserRPNFunction::call(RPNFunctionArgsValue &args, const TextRange & /*range*/,
										ContextPtr context) const {
	ContextPtr const functionContext =
		std::make_shared<Context>(this->name, "", context, CONTEXT_TYPE_FUNCTION);
	for (size_t i = 0; i < args.size(); i++) {
		if (this->arguments.at(i).second.index() == 0) {
			functionContext->setValue(this->arguments.at(i).first,
									  args[i]->copy(Value::CONTEXT_VARIABLE));
		} else {
			functionContext->setValue(
				this->arguments.at(i).first,
				args[i]->to(std::get<ValueType>(this->arguments.at(i).second.getType()),
							Value::CONTEXT_VARIABLE));
		}
	}

	Interpreter interpreter(functionContext);
	ExpressionResult result = interpreter.interpret(this->body->getBlocks());
	if (result.error()) {
		return result;
	}

	// check the return type
	if (!result.returnValue()) {
		if (this->returnType.index() == 0 ||
			(this->returnType.index() == 1 &&
			 std::get<ValueType>(this->returnType.getType()) != NONE)) {
			return ExpressionResult("Function " + this->name + " expected a return value of type " +
										this->returnType.name() + ", but no return value was found",
									this->body->lastRange(), context);
		}

		return None::empty();
	}

	Value *returnValue = interpreter.getLastValue();
	if (this->returnType.index() == 0) {
		if (returnValue->getType() != STRUCT) {
			return ExpressionResult("Return type must be struct of type " +
										this->returnType.name() + " but got " +
										stringType(returnValue->getType()),
									returnValue->getRange(), context);
		}

		if (std::get<std::string>(this->returnType.getType()) !=
			dynamic_cast<Struct *>(returnValue)->getStructName()) {
			return ExpressionResult(
				"Return type must be struct of type " + this->returnType.name() + " but got " +
					std::string(dynamic_cast<Struct *>(returnValue)->getStructName()),
				returnValue->getRange(), context);
		}

		return returnValue->copy();
	}

	if (std::get<ValueType>(this->returnType.getType()) == NONE) {
		return ExpressionResult("Function " + this->name + " does not return any value",
								this->body->lastRange(), context);
	}

	if (!RPNValueType::isCastableTo(returnValue->getType(),
									std::get<ValueType>(this->returnType.getType()))) {
		return ExpressionResult("Return type must be " + this->returnType.name() + " but got " +
									stringType(returnValue->getType()),
								returnValue->getRange(), context);
	}

	return returnValue->to(std::get<ValueType>(this->returnType.getType()));
}

std::shared_ptr<UserRPNFunction> UserRPNFunction::addFunction(const std::string &name,
															  const RPNFunctionArgs &arguments,
															  RPNValueType returnType,
															  CodeBlock *body) {
	userFunctions[name] = std::make_shared<UserRPNFunction>(name, arguments, returnType, body);
	return userFunctions.at(name);
}

std::shared_ptr<UserRPNFunction> UserRPNFunction::getFunction(const std::string &name) {
	if (!userFunctions.contains(name)) {
		return nullptr;
	}
	return userFunctions[name];
}

CodeBlock *UserRPNFunction::getBody() const {
	return this->body;
}

TextRange UserRPNFunction::getRange() const {
	return this->body->getRange();
}

std::unordered_map<std::string, std::shared_ptr<UserRPNFunction>> UserRPNFunction::userFunctions;
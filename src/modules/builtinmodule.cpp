#include "modules/builtinmodule.hpp"

std::unordered_map<std::string, BuiltinRPNFunction> BuiltinModule::builtinFunctions = std::unordered_map<std::string, BuiltinRPNFunction>();

BuiltinModule::BuiltinModule() : isLoaded(false), name(""), loader(nullptr), context(nullptr) {}

BuiltinModule::BuiltinModule(std::string name, loadFunction loader) :
	isLoaded(false),
	name(name), 
	loader(loader), 
	context(new Context(name, "<builtin>", CONTEXT_TYPE_MODULE)) {}

/**
 * @brief load the module if it hasn't been loaded yet
 * 
 * @return ExpressionResult the result of the load operation
 */
ExpressionResult BuiltinModule::load() {
	if (isLoaded) return ExpressionResult();
	this->loader(*this);
	isLoaded = true;

	return ExpressionResult();
}

/**
 * @brief add a function to the module context
 * 
 * @param name the name of the function
 * @param argsName the names of the arguments of the function
 * @param argumentsTypes the types of the arguments of the function
 * @param returnType the return type of the function
 * @param function the function to add
 */
void BuiltinModule::addFunction(
		std::string name,
		std::vector<std::string> argsName,
		std::vector<ValueType> argumentsTypes,
		ValueType returnType,
		BuiltinRPNFunctionType function
) {
	this->builtinFunctions.insert(
		this->builtinFunctions.begin(), 
		std::pair<std::string, BuiltinRPNFunction>(
			name, 
			BuiltinRPNFunction(name, argsName, argumentsTypes, returnType, function)
		)
	);
	context->setValue(
		name,
		new Function(
			static_cast<RPNFunction*>(&this->builtinFunctions[name]),
			TextRange(),
			false
		)
	);
}

/**
 * @brief add a variable to the module context
 * 
 * @param name the name of the variable
 * @param value the value of the variable
 */
void BuiltinModule::addVariable(std::string name, Value *value) {
	context->setValue(name, value);
}

Context * BuiltinModule::getModuleContext() {
	return this->context;
}
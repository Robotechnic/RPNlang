#include "cppmodule/cppmodule.hpp"

std::unordered_map<std::string, BuiltinRPNFunction> CppModule::moduleFunctions = std::unordered_map<std::string, BuiltinRPNFunction>();

CppModule::CppModule() : isLoaded(false), name(""), loader(nullptr), context(nullptr) {}

CppModule::CppModule(std::string name, loadFunction loader) :
	isLoaded(false),
	name(name),
	loader(loader), 
	context(new Context(name, "<builtin-" + name + ">", CONTEXT_TYPE_MODULE)) {}

CppModule::~CppModule() {}

/**
 * @brief load the module if it hasn't been loaded yet
 * 
 * @return ExpressionResult the result of the load operation
 */
ExpressionResult CppModule::load() {
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
void CppModule::addFunction(
		std::string name,
		std::vector<std::string> argsName,
		std::vector<ValueType> argumentsTypes,
		ValueType returnType,
		BuiltinRPNFunctionType function
) {
	this->moduleFunctions.insert(
		this->moduleFunctions.begin(), 
		std::pair<std::string, BuiltinRPNFunction>(
			name, 
			BuiltinRPNFunction(name, argsName, argumentsTypes, returnType, function)
		)
	);
	context->setValue(
		name,
		new Function(
			static_cast<RPNFunction*>(&this->moduleFunctions[name]),
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
void CppModule::addVariable(std::string name, Value *value) {
	context->setValue(name, value);
}

ContextPtr  CppModule::getModuleContext() {
	return this->context;
}

/**
 * @brief check in the rpnModules folder if a module is builtin
 * 
 * @param name the name of the module
 * @return true if the module is builtin
 * @return false if the module is not builtin
 */
bool CppModule::isBuiltin(std::string name) {
	return std::filesystem::exists("rpnModules/" + name + ".so");
}
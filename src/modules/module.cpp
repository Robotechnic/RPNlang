#include "modules/module.hpp"

Module::Module() :
	path(""),
	name(""),
	parentContext(nullptr),
	importRange(TextRange(0, 0, 0)),
	context(nullptr) {}

Module::Module(std::string path, std::string name, const Context * parentContext, TextRange importRange) : 
	path(path), 
	name(name),
	parentContext(parentContext),
	importRange(importRange) {
	context = new Context(name, CONTEXT_TYPE_FILE);
}

Module::Module(const Module &other) : 
	path(other.path), 
	name(other.name),
	parentContext(other.parentContext),
	importRange(other.importRange),
	context(other.context) {}

/**
 * @brief load the module interpret it
 * 
 * @return ExpressionResult if the module is loaded correctly, otherwise quit the program
 */
ExpressionResult Module::load() {
	if (path == "") {
		throw std::runtime_error("Module::load: path is empty");
	}
	Interpreter moduleLoader = Interpreter(context);
	if (!moduleLoader.interpretFile(path)) {
		delete context;
		return ExpressionResult(
			"Failed to load module " + name + " at " + path,
			this->importRange,
			this->parentContext
		);
	}

	return ExpressionResult();
}

/**
 * @brief return the module context
 * 
 * @return Context* the module context
 */
Context * Module::getModuleContext() {
	if (context == nullptr) {
		throw std::runtime_error("Module is not loaded");
	}
	return this->context;
}

/**
 * @brief check if a given string is a module name
 * 
 * @param moduleName the name to check
 * @return bool true if the name is a module name
 */
bool Module::isModule(std::string moduleName) {
	return Module::modules.find(moduleName) != Module::modules.end();
}

/**
 * @brief return the value of a module if it exists
 * 
 * @param pathToken the token containing the path to the module
 * @param value the value to put the module value in
 * @param parentContext the parent context of the module
 * @return ExpressionResult if the value exists
 */
ExpressionResult Module::getModuleValue(const Token &pathToken, Value &value, const Context* context){
	std::vector<std::string> path = split(pathToken.getValue(), '.');
	if (path.size() > 2) {
		return ExpressionResult(
			"Maximum path depth is 2",
			pathToken.getRange(),
			context
		);
	}

	if (!Module::isModule(path[0])) {
		return ExpressionResult(
			"Module '" + path[0] + "' does not exist",
			pathToken.getRange(),
			context
		);
	}
	
	return Module::modules.at(path[0]).getModuleContext()->getValue(pathToken, path[1], value);
}

/**
 * @brief add a module to the module list and load it
 * 
 * @param modulePath the path to the module
 * @param name the name of the module
 * @param importRange the range of the import statement
 * @param parentContext the parent context of the module import
 * @return ExpressionResult if the module is loaded correctly
 */
ExpressionResult Module::addModule(std::string modulePath, std::string name, TextRange importRange, const Context* parentContext) {
	Module::modules[name] = Module(modulePath, name, parentContext, importRange);
	return Module::modules[name].load();
}

std::map<std::string, Module>Module::modules = std::map<std::string, Module>();
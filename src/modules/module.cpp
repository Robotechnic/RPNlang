#include "modules/module.hpp"

Module::Module() :
	path(""),
	name(""),
	importRange(TextRange(0, 0, 0)),
	context(nullptr) {}

Module::Module(std::string path, std::string name, const Context * parentContext, TextRange importRange) : 
	path(path), 
	name(name),
	importRange(importRange) {
	context = new Context(name, parentContext, CONTEXT_TYPE_MODULE);
}

Module::Module(const Module &other) : 
	path(other.path), 
	name(other.name),
	importRange(other.importRange),
	context(other.context) {}

Module::~Module() {
	if (this->context != nullptr) {
		// delete this->context;
	}
}

/**
 * @brief load the module interpret it
 * 
 * @return ExpressionResult if the module is loaded correctly, otherwise quit the program
 */
ExpressionResult Module::load() {
	if (path == "") {
		throw std::runtime_error("Module::load: path is empty");
	}

	if (this->context == nullptr) {
		throw std::runtime_error("Module::load: context is null");
	}

	Interpreter moduleLoader = Interpreter(this->context);
	std::string error;
	if (moduleLoader.interpretFile(path, error)) return ExpressionResult();
	
	if (Module::builtinModules.find(path) != Module::builtinModules.end()) {
		Module::builtinModules[path].load();
		const Context *parent = this->context->getParent();
		delete this->context;
		this->context = Module::builtinModules[path].getModuleContext();
		this->context->setParent(parent);

		return ExpressionResult();
	}
	
	return ExpressionResult(
		"Failed to load module " + name + " at " + path + "(" + error + ")",
		this->importRange,
		this->context->getParent()
	);
}

/**
 * @brief return the module context
 * 
 * @return Context *the module context
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
ExpressionResult Module::getModuleValue(Value *value, const Context *context) {
	std::vector<std::string> path = split(value->getStringValue(), '.');
	if (path.size() > 2) {
		return ExpressionResult(
			"Maximum path depth is 2",
			value->getRange(),
			context
		);
	}

	if (!Module::isModule(path[0])) {
		return ExpressionResult(
			"Module '" + path[0] + "' does not exist",
			value->getRange(),
			context
		);
	}
	
	return Module::modules.at(path[0]).getModuleContext()->getValue(value, path[1], value);
}

ExpressionResult Module::getModuleValue(const Token &pathToken, Value *value, const Context *parentContext) {
	std::vector<std::string> path = split(pathToken.getValue(), '.');
	if (path.size() > 2) {
		return ExpressionResult(
			"Maximum path depth is 2",
			pathToken.getRange(),
			parentContext
		);
	}

	if (!Module::isModule(path[0])) {
		return ExpressionResult(
			"Module '" + path[0] + "' does not exist",
			pathToken.getRange(),
			parentContext
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
ExpressionResult Module::addModule(std::string modulePath, std::string name, TextRange importRange, const Context *parentContext) {
	Module::modules[name] = Module(modulePath, name, parentContext, importRange);
	return Module::modules[name].load();
}

std::map<std::string, Module>Module::modules = std::map<std::string, Module>();

std::map<std::string, BuiltinModule>Module::builtinModules = std::map<std::string, BuiltinModule>{
	{"test", BuiltinModule("test", [](BuiltinModule &module) {
		module.addFunction("testFunction", {"value"}, {STRING}, NONE, [](RPNFunctionArgs args, Context *context) {
			std::cout<<"Test ok : "<<args[0]->getStringValue()<<std::endl;
			return std::make_tuple(ExpressionResult(), nullptr);
		});

		module.addVariable("testValue",new String(
			std::string("testValueOk"),
			TextRange()
		));
		return ExpressionResult();
	})},

	{"math", BuiltinModule("math", mathLoader)},

	{"time", BuiltinModule("time", timeLoader)}
};
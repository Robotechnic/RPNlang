#include "modules/module.hpp"

Module::Module() :
	path(""),
	name(""),
	importRange(TextRange(0, 0, 0)),
	context(nullptr) {}

Module::Module(std::string path, std::string name, ContextPtr  parentContext, TextRange importRange) : 
	path(path), 
	name(name),
	importRange(importRange),
	context(std::make_shared<Context>(name, path, parentContext, CONTEXT_TYPE_MODULE, true))
{}

Module::Module(const Module &other) : 
	path(other.path), 
	name(other.name),
	importRange(other.importRange),
	context(other.context) {}

Module::~Module() {}

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
	if (!moduleLoader.interpretFile(path, error))
		return ExpressionResult(
			"Failed to load module " + name + " at " + path + "(" + error + ")",
			this->importRange,
			this->context->getParent()
		);

	return ExpressionResult();
}

/**
 * @brief return the module context
 * 
 * @return ContextPtr the module context
 */
ContextPtr Module::getModuleContext() const {
	if (context.use_count() == 0) {
		throw std::runtime_error("Module is not loaded");
	}
	return this->context;
}

std::string Module::getPath() const {
	return this->path;
}

/**
 * @brief check if a given string is a module name
 * 
 * @param moduleName the name to check
 * @return bool true if the name is a module name
 */
bool Module::isModule(std::string moduleName) {
	return (
		Module::modules.find(moduleName) != Module::modules.end() || 
		Module::builtinModules.find(moduleName) != Module::builtinModules.end()
	);
}

/**
 * @brief check if a given path is correct or not
 * 
 * @param path the path to check
 * @return std::string contains the error message, empty if there is not
 */
std::string Module::checkPath(std::vector<std::string> path) {
	if (path.size() > 2) {
		return "Maximum path depth is 2";
	}

	if (!Module::isModule(path[0])) {
		return "Module '" + path[0] + "' does not exist";
	}

	return "";
}

/**
 * @brief return the value of a module if it exists
 * 
 * @param valuePath the token containing the path to the module
 * @param value the value to put the module value in
 * @param parentContext the parent context of the module
 * @return ExpressionResult if the value exists
 */
ExpressionResult Module::getModuleValue(const Value *valuePath, Value *&value, const ContextPtr &parentContext) {
	std::vector<std::string> path = static_cast<const Path*>(valuePath)->getPath();
	std::string error = Module::checkPath(path);
	if (error != "") {
		return ExpressionResult(
			error,
			valuePath->getRange(),
			parentContext
		);
	}

	if (Module::modules.find(path[0]) != Module::modules.end())
		return Module::modules.at(path[0])->getModuleContext()->getModuleValue(path, valuePath->getRange(), value);
	else if (Module::builtinModules.find(path[0]) != Module::builtinModules.end())
		return Module::builtinModules.at(path[0]).getModuleContext()->getModuleValue(path, valuePath->getRange(), value);
	else
		throw std::runtime_error("Module::getModuleValue: module not found");
}

ExpressionResult Module::getModuleValue(const Token *tokenPath, Value *&value, const ContextPtr &parentContext) {
	std::vector<std::string> path = split(tokenPath->getStringValue(), '.');
	std::string error = Module::checkPath(path);
	if (error != "") {
		return ExpressionResult(
			error,
			tokenPath->getRange(),
			parentContext
		);
	}

	if (Module::modules.find(path[0]) != Module::modules.end())
		return Module::modules.at(path[0])->getModuleContext()->getModuleValue(path, tokenPath->getRange(), value);
	else if (Module::builtinModules.find(path[0]) != Module::builtinModules.end())
		return Module::builtinModules.at(path[0]).getModuleContext()->getModuleValue(path, tokenPath->getRange(), value);
	else
		throw std::runtime_error("Module::getModuleValue: module not found");
}


/**
 * @brief return the module context
 * 
 * @param pathToken the token containing the path to the module
 * @param parentContext the parent context of the module
 * @param moduleContext the module context to put the module context in
 * @return ExpressionResult 
 */
ExpressionResult Module::getModuleContext(const Token *tokenPath, const ContextPtr &parentContext, ContextPtr &moduleContext) {
	std::vector<std::string> path = split(tokenPath->getStringValue(), '.');
	std::string error = Module::checkPath(path);
	if (error != "") {
		return ExpressionResult(
			error,
			tokenPath->getRange(),
			parentContext
		);
	}
	if (Module::modules.find(path[0]) != Module::modules.end())
		moduleContext = Module::modules.at(path.at(0))->getModuleContext();
	else if (Module::builtinModules.find(path[0]) != Module::builtinModules.end())
		moduleContext = Module::builtinModules.at(path.at(0)).getModuleContext();
	else
		throw std::runtime_error("Module::getModuleContext: module not found");
	return ExpressionResult();
}

/**
 * @brief check if a given file is already loaded to avoid loading it twice or bugs with mutual imports
 * 
 * @param modulePath the path to check
 * @return bool true if the file is already loaded
 */
bool Module::isImported(std::string modulePath, std::string &moduleName) {
	auto it = std::find_if(Module::modules.begin(), Module::modules.end(), [modulePath](auto module) -> bool {
		return std::filesystem::equivalent(std::get<1>(module)->getPath(), modulePath);
	});
	if (it == Module::modules.end()) 
		return false;
	
	moduleName = it->first;
	return true;
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
ExpressionResult Module::addModule(std::string modulePath, std::string name, TextRange importRange, const ContextPtr &parentContext) {	
	std::string importedName;
	// check if the module is user defined
	if (modulePath != name) {
		if (Module::isImported(modulePath, importedName)) {
			if (importedName != name)
				Module::modules[name] = Module::modules[importedName];
			return ExpressionResult();
		}
		Module::modules[name] = std::make_shared<Module>(modulePath, name, parentContext, importRange);
		return Module::modules[name]->load();
	}

	// check if the module is builtin
	if (!CppModule::isBuiltin(name)) {
		return ExpressionResult(
			"Builtin module '" + name + "' does not exist",
			importRange,
			parentContext
		);
	}

	builtinModules[name] = CppModule(name);
	return builtinModules[name].load(importRange);
}


std::unordered_map<std::string, std::shared_ptr<Module>>Module::modules = std::unordered_map<std::string, std::shared_ptr<Module>>();
std::unordered_map<std::string, CppModule>Module::builtinModules = std::unordered_map<std::string, CppModule>();

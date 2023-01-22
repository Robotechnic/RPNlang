#include "modules/module.hpp"

Module::Module() :
	path(""),
	name(""),
	importRange(TextRange(0, 0, 0)),
	context(nullptr) {}

Module::Module(std::string_view path, std::string_view name, ContextPtr  parentContext, TextRange importRange) : 
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
	if (!moduleLoader.interpretFile(path, error, true))
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
 * @param isBuiltin if the module is builtin
 * @return bool true if the name is a module name
 */
bool Module::isModule(const std::string &moduleName, bool &isBuiltin) {
	isBuiltin = false;
	return (
		Module::modules.find(moduleName) != Module::modules.end() || 
		(isBuiltin = Module::builtinModules.find(moduleName) != Module::builtinModules.end())
	);
}

/**
 * @brief check if a given path is correct or not
 * 
 * @param path the path to check
 * @param parentContext the parent context of the module
 * @param isBuiltin if the module is builtin
 * @return std::string contains the error message, empty if there is not
 */
ExpressionResult Module::checkPath(const Path* path, const ContextPtr &parentContext, bool &isBuiltin) {
	if (path->size() > 2) {
		return ExpressionResult(
			"Module path can only be 2 tokens long",
			path->getRange(),
			parentContext
		);
	}

	if (!Module::isModule(path->ats(0), isBuiltin)) {
		return ExpressionResult(
			"Module " + path->ats(0) + " does not exist",
			path->getRange(),
			parentContext
		);
	}

	return ExpressionResult();
}

/**
 * @brief return the value of a module if it exists
 * 
 * @param valuePath the token containing the path to the module
 * @param value the value to put the module value in
 * @param parentContext the parent context of the module
 * @return ExpressionResult if the value exists
 */
Value *&Module::getModuleValue(const Value *valuePath) {
	const Path* path = static_cast<const Path*>(valuePath);

	if (path->getType() == PATH)
		return Module::modules.at(path->ats(0))->getModuleContext()->getValue(path->ats(1));
	else if (path->getType() == BUILTIN_PATH)
		return Module::builtinModules.at(path->ats(0)).getModuleContext()->getValue(path->ats(1));
	else
		throw std::runtime_error("This path is not a module path");
}

/**
 * @brief return the module context
 * 
 * @param pathToken the token containing the path to the module
 * @param parentContext the parent context of the module
 * @param moduleContext the module context to put the module context in
 * @return ExpressionResult 
 */
ContextPtr Module::getModuleContext(const Value *valuePath, const ContextPtr &parentContext) {
	const Path* path = static_cast<const Path*>(valuePath);
	ContextPtr moduleContext;
	if (path->getType() == PATH)
		moduleContext = Module::modules.at(path->ats(0))->getModuleContext();
	else if (path->getType() == BUILTIN_PATH)
		moduleContext = Module::builtinModules.at(path->ats(0)).getModuleContext();
	else
		throw std::runtime_error("This path is not a module path");
	moduleContext->setParent(parentContext);
	return moduleContext;
}

/**
 * @brief check if a given file is already loaded to avoid loading it twice or bugs with mutual imports
 * 
 * @param modulePath the path to check
 * @return bool true if the file is already loaded
 */
bool Module::isImported(std::string_view modulePath, std::string &moduleName) {
	auto it = std::find_if(Module::modules.begin(), Module::modules.end(), [modulePath](auto module) -> bool {
		return std::filesystem::equivalent(module.second->getPath(), modulePath);
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
ExpressionResult Module::addModule(std::string_view modulePath, const std::string &name, TextRange importRange, const ContextPtr &parentContext) {	
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

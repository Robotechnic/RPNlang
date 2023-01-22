#include "cppmodule/cppmodule.hpp"

std::unordered_map<std::string, BuiltinRPNFunction> CppModule::moduleFunctions = std::unordered_map<std::string, BuiltinRPNFunction>();
std::string CppModule::builtinModulesPath = "";
unsigned int CppModule::openModulesCount = 0;

CppModule::CppModule() : name(""), context(nullptr), handle(nullptr) {}

CppModule::CppModule(std::string_view name) :
	name(name),
	context(new Context(name, "<builtin-" + std::string(name) + ">", CONTEXT_TYPE_BUILTIN_MODULE)), 
	handle(nullptr) {}

CppModule::~CppModule() {
	if (!this->handle) {
		return;
	}
	CppModule::openModulesCount--;
	if (CppModule::openModulesCount < 0) {
		if(dlclose(this->handle) != 0) {
			std::cerr<<"Error while closing module "<<name<<": "<<dlerror()<<std::endl;
		}
	}
}

/**
 * @brief load the module if it hasn't been loaded yet
 * 
 * @return ExpressionResult the result of the load operation
 */
ExpressionResult CppModule::load(TextRange imortRange) {
	CppModule::openModulesCount++;
	this->handle = dlopen((CppModule::builtinModulesPath + "/lib" + name + ".so").c_str(), RTLD_LAZY);
	if (!this->handle) {
		return ExpressionResult(
			"Error while loading module " + name + ": " + dlerror(),
			imortRange,
			this->context
		);
	}
	dlerror();

	ModuleAPI *api = (ModuleAPI*)dlsym(this->handle, "moduleAPI");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		return ExpressionResult(
			"Error while loading module " + name + ": " + dlsym_error,
			imortRange,
			this->context
		);
	}
	if (!api) {
		return ExpressionResult(
			"Error while loading module " + name + ": moduleAPI is null",
			imortRange,
			this->context
		);
	}
	api->loader(this);
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
		const std::string &name,
		const RPNFunctionArgs &arguments,
		RPNValueType returnType,
		BuiltinRPNFunctionType function
) {
	this->moduleFunctions.insert(
		this->moduleFunctions.begin(), 
		std::pair<std::string, BuiltinRPNFunction>(
			name,
			BuiltinRPNFunction(name, arguments, returnType, function)
		)
	);
	context->setValue(
		name,
		new Function(
			static_cast<RPNFunction*>(&this->moduleFunctions[name]),
			TextRange(),
			Value::CONTEXT_VARIABLE
		)
	);
}

/**
 * @brief add a variable to the module context
 * 
 * @param name the name of the variable
 * @param value the value of the variable
 */
void CppModule::addVariable(const std::string &name, Value *value) {
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
bool CppModule::isBuiltin(const std::string &name) {
	return std::filesystem::exists(CppModule::builtinModulesPath + "/lib" + name + ".so");
}

/**
 * @brief set the path to the rpnModules folder
 * 
 * @param path the path to the rpnModules folder
 */
void CppModule::setBuiltinModulesPath(const std::string &path) {
	CppModule::builtinModulesPath = path;
}
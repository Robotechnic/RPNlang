#pragma once

#include <dlfcn.h>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "rpnfunctions/typedef.hpp"
#include "value/types/function.hpp"
#include "value/value.hpp"

class CppModule;
typedef std::function<ExpressionResult(CppModule *)> loadFunction;

class BuiltinRPNFunction;

struct ModuleAPI {
	// this fieds will be used in future versions
	const char *name;
	const char *description;
	const char *version;
	const char *author;
	// this is the only required field
	loadFunction loader;
};

class CppModule {
  public:
	CppModule();
	CppModule(std::string_view name);
	~CppModule();

	ExpressionResult load(TextRange imortRange);

	void addFunction(const std::string &name, const RPNFunctionArgs &arguments,
					 RPNValueType returnType, BuiltinRPNFunctionType function);
	void addVariable(const std::string &name, Value *value);

	ContextPtr getModuleContext();
	static bool isBuiltin(const std::string &name);
	static void setBuiltinModulesPath(const std::string &path);

  private:
	std::string name;
	ContextPtr context;
	void *handle;

	static unsigned int openModulesCount;
	static std::string builtinModulesPath;
	static std::unordered_map<std::string, BuiltinRPNFunction> moduleFunctions;
};
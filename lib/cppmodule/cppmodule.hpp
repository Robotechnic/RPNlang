#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <dlfcn.h>

#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "value/value.hpp"
#include "value/types/function.hpp"

class CppModule;
typedef std::function<ExpressionResult(CppModule*)> loadFunction;

class BuiltinRPNFunction;

struct ModuleAPI {
	// this fieds will be used in future versions
	const char* name;
	const char* description;
	const char* version;
	const char* author;
	// this is the only required field
	loadFunction loader;
};

class CppModule {
	public:
		CppModule();
		CppModule(std::string name);
		~CppModule();

		ExpressionResult load(TextRange imortRange);

		void addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		);
		void addVariable(std::string name, Value *value);

		ContextPtr getModuleContext();
		static bool isBuiltin(std::string name);
		static void setBuiltinModulesPath(std::string path);

	private:
		std::string name;
		ContextPtr context;
		void *handle;
		
		static unsigned int openModulesCount;
		static std::string builtinModulesPath;
		static std::unordered_map<std::string, BuiltinRPNFunction> moduleFunctions;
};
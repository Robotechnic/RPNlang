#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "value/value.hpp"
#include "value/types/function.hpp"

class CppModule;
typedef std::function<ExpressionResult(CppModule&)> loadFunction;

class BuiltinRPNFunction;

struct ModuleAPI {
	std::string name;
	std::string description;
	std::string version;
	std::string author;
	loadFunction loader;
};

class CppModule {
	public:
		CppModule();
		CppModule(std::string name, loadFunction loader);
		~CppModule();

		ExpressionResult load();

		void addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		);
		void addVariable(std::string name, Value *value);

		ContextPtr  getModuleContext();
		static bool isBuiltin(std::string name);

	private:
		bool isLoaded;
		std::string name;
		loadFunction loader;
		ContextPtr context;

		static std::unordered_map<std::string, BuiltinRPNFunction> moduleFunctions;
};
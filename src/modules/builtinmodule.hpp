#pragma once

#include <string>
#include <functional>
#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "value/value.hpp"

class BuiltinModule;
typedef std::function<ExpressionResult(BuiltinModule&)> loadFunction;

class BuiltinModule {
	public:
		BuiltinModule();
		BuiltinModule(std::string name, loadFunction loader);

		ExpressionResult load();

		void addFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		);
		void addVariable(std::string name, Value *value);

		Context * getModuleContext();

	private:
		bool isLoaded;
		std::string name;
		loadFunction loader;
		Context *context;
};
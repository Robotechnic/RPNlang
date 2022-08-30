#pragma once

#include <stdexcept>

class BuiltinModule;

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "textutilities/textrange.hpp"
#include "interpreter/interpreter.hpp"
#include "rpnfunctions/typedef.hpp"
#include "modules/builtinmodule.hpp"

// modules
#include "modules/math/math.hpp"
#include "modules/time/time.hpp"

class Module {
	public:
		Module();
		Module(std::string path, std::string name, const Context * parentContext, TextRange importRange);
		Module(const Module &other);
		~Module();

		ExpressionResult load();

		Context * getModuleContext();

		static bool isModule(std::string moduleName);
		static ExpressionResult addModule(std::string modulePath, std::string name, TextRange importRange, const Context *context);
		static ExpressionResult getModuleValue(const Value *path, Value *&value, const Context *parentContext);
		static ExpressionResult getModuleValue(const Token &pathToken, Value *&value, const Context *parentContext);

	private:
		std::string path;
		std::string name;
		TextRange importRange;
		Context * context;

		static std::map<std::string, Module> modules;
		static std::map<std::string, BuiltinModule> builtinModules;
};


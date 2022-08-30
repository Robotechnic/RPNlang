#pragma once

#include <stdexcept>
#include <filesystem>

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
#include "modules/random/random.hpp"

class Module {
	public:
		Module();
		Module(std::string path, std::string name, const Context * parentContext, TextRange importRange);
		Module(const Module &other);
		~Module();

		ExpressionResult load();

		Context * getModuleContext();
		std::string getPath() { return path; };

		static bool isModule(std::string moduleName);
		static bool isImported(std::string modulePath, std::string &moduleName);

		static std::string checkPath(std::vector<std::string> path);
		static ExpressionResult addModule(std::string modulePath, std::string name, TextRange importRange, const Context *context);
		static ExpressionResult getModuleValue(const Value *path, Value *&value, const Context *parentContext);
		static ExpressionResult getModuleValue(const Token &pathToken, Value *&value, const Context *parentContext);
		static ExpressionResult getModuleContext(const Token &pathToken, const Context *parentContext, Context *&moduleContext);

	private:
		std::string path;
		std::string name;
		TextRange importRange;
		Context * context;

		static std::map<std::string, std::shared_ptr<Module>> modules;
		static std::map<std::string, BuiltinModule> builtinModules;
};


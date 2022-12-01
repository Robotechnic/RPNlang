#pragma once

#include <stdexcept>
#include <unordered_map>
#include <filesystem>

class BuiltinModule;

#include "value/value.hpp"
#include "value/types/path.hpp"

#include "expressionresult/expressionresult.hpp"
#include "textutilities/textrange.hpp"
#include "interpreter/interpreter.hpp"
#include "rpnfunctions/typedef.hpp"
#include "modules/builtinmodule.hpp"
#include "context/context.hpp"

// builtin modules
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
		static ExpressionResult getModuleValue(const Value *valuePath, Value *&value, const Context *parentContext);
		static ExpressionResult getModuleContext(const Value *valuePath, const Context *parentContext, Context *&moduleContext);

	private:
		std::string path;
		std::string name;
		TextRange importRange;
		Context * context;

		static std::unordered_map<std::string, std::shared_ptr<Module>> modules;
		static std::unordered_map<std::string, BuiltinModule> builtinModules;
};


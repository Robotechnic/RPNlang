#pragma once

#include <stdexcept>
#include <unordered_map>
#include <filesystem>
#include <string_view>

class CppModule;

#include "value/value.hpp"
#include "value/types/path.hpp"

#include "expressionresult/expressionresult.hpp"
#include "textutilities/textrange.hpp"
#include "interpreter/interpreter.hpp"
#include "rpnfunctions/typedef.hpp"
#include "cppmodule/cppmodule.hpp"
#include "context/context.hpp"


class Module {
	public:
		Module();
		Module(std::string_view path, std::string_view name, const ContextPtr  parentContext, TextRange importRange);
		Module(const Module &other);
		~Module();

		ExpressionResult load();

		ContextPtr  getModuleContext() const;
		std::string getPath() const;

		static bool isModule(const std::string &moduleName);
		static bool isImported(std::string_view modulePath, std::string &moduleName);

		static std::string checkPath(std::vector<std::string> path);
		static ExpressionResult addModule(std::string_view modulePath, const std::string &name, TextRange importRange, const ContextPtr &context);
		static ExpressionResult getModuleValue(const Token *tokenPath, Value *&value, const ContextPtr &parentContext);
		static ExpressionResult getModuleValue(const Value *valuePath, Value *&value, const ContextPtr &parentContext);
		static ExpressionResult getModuleContext(const Token *tokenPath, const ContextPtr &parentContext, ContextPtr &moduleContext);

	private:
		std::string path;
		std::string name;
		TextRange importRange;
		ContextPtr context;

		static std::unordered_map<std::string, std::shared_ptr<Module>> modules;
		static std::unordered_map<std::string, CppModule> builtinModules;
};


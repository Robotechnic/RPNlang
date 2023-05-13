#pragma once

#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

class CppModule;

#include "value/types/path.hpp"
#include "value/value.hpp"

#include "context/context.hpp"
#include "cppmodule/cppmodule.hpp"
#include "expressionresult/expressionresult.hpp"
#include "interpreter/interpreter.hpp"
#include "rpnfunctions/typedef.hpp"
#include "textutilities/textrange.hpp"

class Module {
  public:
	Module();
	Module(std::string_view path, std::string_view name, const ContextPtr parentContext,
		   TextRange importRange);
	Module(const Module &other);
	~Module();

	ExpressionResult load();

	ContextPtr getModuleContext() const;
	std::string getPath() const;

	static bool isModule(const std::string &moduleName, bool &isBuiltin);
	static bool isImported(std::string_view modulePath, std::string &moduleName);
	static bool hasValue(const Path *path);

	static ExpressionResult checkPath(const Path *path, const ContextPtr &parentContext,
									  bool &isBuiltin);
	static ExpressionResult addModule(std::string_view modulePath, const std::string &name,
									  TextRange importRange, const ContextPtr &context);
	static Value *&getModuleValue(const Value *valuePath);
	static ContextPtr getModuleContext(const Value *valuePath, const ContextPtr &parentContext);

  private:
	std::string path;
	std::string name;
	TextRange importRange;
	ContextPtr context;

	static std::unordered_map<std::string, std::shared_ptr<Module>> modules;
	static std::unordered_map<std::string, CppModule> builtinModules;
};

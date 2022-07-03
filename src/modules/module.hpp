#pragma once

#include <stdexcept>

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "textutilities/textrange.hpp"
#include "interpreter/interpreter.hpp"

class Module {
	public:
		Module();
		Module(std::string path, std::string name, const Context * parentContext, TextRange importRange);
		Module(const Module &other);
		~Module();

		ExpressionResult load();

		Context * getModuleContext();

		static bool isModule(std::string moduleName);
		static ExpressionResult addModule(std::string modulePath, std::string name, TextRange importRange, const Context* context);
		static ExpressionResult getModuleValue(Value &value, const Context* parentContext);

	private:
		std::string path;
		std::string name;
		TextRange importRange;
		Context * context;

		static std::map<std::string, Module> modules;
};


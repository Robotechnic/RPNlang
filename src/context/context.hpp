#pragma once
#include <map>
#include <string>
#include <ostream>
#include "context/contexttypes.hpp"
#include "value/value.hpp"
#include "value/valuetype.hpp"
#include "expressionresult/expressionresult.hpp"

typedef std::map<std::string, Value> symbolTable;

class Context {
	public:
		Context();
		Context(std::string name, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		
		void setName(std::string name);
		std::string getName() const;

		void setParent(Context* parent);
		Context* getParent() const;

		ContextType getType() const;

		void setValue(const Token &name, Value value);
		ExpressionResult getValue(const Token &name, Value &value) const;

	private:
		std::string name;
		symbolTable symbols;
		ContextType type;
		Context* parent;
};


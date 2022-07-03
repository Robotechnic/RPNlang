#pragma once

#include <map>
#include <string>
#include <ostream>
#include "context/contexttypes.hpp"
#include "expressionresult/expressionresult.hpp"
#include "tokens/token.hpp"
#include "value/value.hpp"

class Token;
class Value;
class ExpressionResult;

typedef std::map<std::string, Value> symbolTable;

class Context {
	public:
		Context(const Context &other);
		Context(std::string name, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, const Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, const Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		~Context();

		void setName(std::string name);
		std::string getName() const;

		void setParent(const Context* parent);
		void setChild(Context* child);
		Context *getChild() const;
		const Context* getParent() const;

		ContextType getType() const;

		void setValue(std::string name, Value value);
		void setValue(const Token &name, Value value);
		void setValue(const Value &name, Value value);
		
		ExpressionResult getValue(const Value &name, Value &value) const;
		ExpressionResult getValue(const Token &name, Value &value) const;
		ExpressionResult getValue(const Value &path, const std::string name, Value &value) const;
		ExpressionResult getValue(const std::string &name, Value &value) const;
		bool hasValue(const std::string name) const;
		Value getValue(const Value &name) const;
		Value getValue(const Token &name) const;
		Value getValue(const std::string name) const;
		

	private:
		std::string name;
		symbolTable symbols;
		ContextType type;
		const Context* parent;
		Context* child; // save a pointer to the child to save it and have the possibility to delete it
};

std::ostream& operator<<(std::ostream& os, const Context* context);
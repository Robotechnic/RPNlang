#pragma once

#include <map>
#include <string>
#include <ostream>
#include "context/contexttypes.hpp"

class Token;
class Value;
class ExpressionResult;

typedef std::map<std::string, Value> symbolTable;

class Context {
	public:
		Context();
		Context(const Context &other);
		Context(std::string name, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, const Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, symbolTable symbols, const Context* parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		
		void setName(std::string name);
		std::string getName() const;

		void setParent(Context* parent);
		const Context* getParent() const;

		ContextType getType() const;

		void setValue(std::string name, Value value);
		void setValue(const Token &name, Value value);
		void setValue(const Value &name, Value value);
		
		ExpressionResult getValue(const Value &name, Value &value) const;
		ExpressionResult getValue(const Token &name, Value &value) const;
		Value getValue(const Value &name) const;
		Value getValue(const Token &name) const;


		void clear();

	private:
		std::string name;
		symbolTable symbols;
		ContextType type;
		const Context* parent;
};

std::ostream& operator<<(std::ostream& os, const Context& context);

// I don't know why, but I must include this here else the compiler will not compile the code
#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "tokens/token.hpp"
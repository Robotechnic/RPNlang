#pragma once

#include <unordered_map>
#include <string>
#include <ostream>
#include "tokens/token.hpp"
#include "context/contexttypes.hpp"
#include "value/value.hpp"

class Value;
class ExpressionResult;

typedef std::unordered_map<std::string, Value*> symbolTable;

class Context {
	public:
		Context(const Context &other);
		Context(const Context *other);
		Context(std::string name, std::string filePath, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, std::string filePath, const Context *parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, std::string filePath, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, std::string filePath, symbolTable symbols, const Context *parent, ContextType type = CONTEXT_TYPE_DEFAULT);
		~Context();

		void setName(std::string name);
		std::string getName() const;

		void setFilePath(std::string filePath);
		std::string getFilePath() const;

		void setParent(const Context *parent);
		void setChild(Context *child);
		void clearChild();

		Context *getChild() const;
		const Context *getParent() const;

		ContextType getType() const;

		void setValue(std::string name, Value *value);
		void setValue(const Token *name, Value *value);
		void setValue(const Value &name, Value *value);
		
		ExpressionResult getValue(const Value *name, Value *&value) const;
		ExpressionResult getValue(const Token *name, Value *&value) const;
		ExpressionResult getValue(const Value *path, const std::string name, Value *&value) const;
		Value *getValue(const Value *name) const;
		Value *getValue(const std::string &name) const;

		bool hasValue(const std::string name) const;
		

	private:
		std::string name, filePath;
		symbolTable symbols;
		ContextType type;
		const Context *parent;
		const Context *root;
		Context *child; // save a pointer to the child to have the possibility to delete it
};

std::ostream& operator<<(std::ostream& os, const Context *context);

#include "expressionresult/expressionresult.hpp"
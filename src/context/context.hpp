#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <ostream>
#include "tokens/token.hpp"
#include "context/contexttypes.hpp"
#include "context/typedef.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"

class Value;
typedef std::unordered_map<std::string, Value*> symbolTable;

class Context : public std::enable_shared_from_this<Context> {
	public:
		Context(const Context &other);
		Context(const Context *other);
		Context(std::string name, std::string filePath, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, std::string filePath, ContextPtr parent, ContextType type = CONTEXT_TYPE_DEFAULT, bool root = false);
		Context(std::string name, std::string filePath, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string name, std::string filePath, symbolTable symbols, ContextPtr parent, ContextType type = CONTEXT_TYPE_DEFAULT, bool root = false);
		~Context();

		void setName(std::string name);
		std::string getName() const;

		void setFilePath(std::string filePath);
		std::string getFilePath() const;

		void setParent(ContextPtr parent);
		void clearChild();

		ContextPtr getParent() const;
		ContextType getType() const;

		void setValue(std::string name, Value *value, Value **hold = nullptr);
		void setValue(const Token *name, Value *value, Value **hold = nullptr);
		void setValue(const Value *name, Value *value, Value **hold = nullptr);
		
		ExpressionResult getValue(const Value *name, Value *&value);
		ExpressionResult getValue(const Token *name, Value *&value);
		ExpressionResult getModuleValue(std::vector<std::string> path, TextRange range, Value *&value);
		Value *getValue(const Value *name);
		Value *getValue(const std::string &name);

		bool hasValue(const std::string name) const;
		

	private:
		std::string name, filePath;
		symbolTable symbols;
		ContextType type;
		ContextPtr parent;
		ContextPtr root;
};

std::ostream& operator<<(std::ostream& os, const ContextPtr &context);
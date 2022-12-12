#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <ostream>
#include <string_view>
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
		Context(std::string_view name, std::string_view filePath, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string_view name, std::string_view filePath, ContextPtr parent, ContextType type = CONTEXT_TYPE_DEFAULT, bool root = false);
		Context(std::string_view name, std::string_view filePath, symbolTable symbols, ContextType type = CONTEXT_TYPE_DEFAULT);
		Context(std::string_view name, std::string_view filePath, symbolTable symbols, ContextPtr parent, ContextType type = CONTEXT_TYPE_DEFAULT, bool root = false);
		~Context();

		void clear();

		inline void setName(std::string_view name);
		std::string getName() const;

		inline void setFilePath(std::string_view filePath);
		std::string getFilePath();

		void setParent(ContextPtr parent);

		ContextPtr getParent() const;
		ContextType getType() const;

		void setValue(const std::string &name, Value *value, Value **hold = nullptr, bool takeOwnership = false);
		void setValue(const Token *name, Value *value, Value **hold = nullptr, bool takeOwnership = false);
		void setValue(const Value *name, Value *value, Value **hold = nullptr, bool takeOwnership = false);
		
		ExpressionResult getValue(const Value *name, Value *&value);
		ExpressionResult getValue(const Token *name, Value *&value);
		ExpressionResult getModuleValue(std::vector<std::string> path, TextRange range, Value *&value);
		Value *getValue(const Value *name);
		Value *getValue(const std::string &name);

		bool hasValue(std::string_view name) const;
		void takeOwnership();

	private:
		std::string name, filePath;
		symbolTable symbols;
		ContextType type;
		ContextPtr parent;
		ContextPtr root;
};

std::ostream& operator<<(std::ostream& os, const ContextPtr &context);
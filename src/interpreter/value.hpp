#pragma once

#include <string>
#include <stdexcept>
#include "tokens/token.hpp"
#include "interpreter/expressionresult.hpp"

enum ValueType {
	INT,
	FLOAT,
	STRING,
	VARIABLE
};

class ExpressionResult;

class Value {
	public:
		Value();
		Value(std::string value, ValueType type, int line, int column);
		Value(std::string value, TokenType type, int line, int column);

		ValueType getType() const;
		std::string getValue() const;

		static std::string stringType(ValueType type);

		ExpressionResult setVariable(std::map<std::string, Value> &variables);

		ExpressionResult add(const Value &other);
		ExpressionResult substract(const Value &other);
		ExpressionResult multiply(const Value &other);
		ExpressionResult divide(const Value &other);


	private:
		ExpressionResult stringTest(const Value &other);
		int line, column;
		std::string value;
		ValueType type;
};
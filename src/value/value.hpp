#pragma once

class ExpressionResult;

#include <string>
#include <stdexcept>
#include <tuple>
#include "tokens/tokentypes.hpp"
#include "tokens/token.hpp"
#include "textrange/textrange.hpp"
#include "value/valuetype.hpp"


class Value {
	public:
		Value();
		Value(const Value &other);
		Value(std::string value, ValueType type, int line, int column);
		Value(std::string value, TokenType type, int line, int column);

		ValueType getType() const;
		std::string getValue() const;
		TextRange getRange() const;

		static std::string stringType(ValueType type);

		ExpressionResult setVariable(std::map<std::string, Value> &variables);

		ExpressionResult add(const Value &other);
		ExpressionResult substract(const Value &other);
		ExpressionResult multiply(const Value &other);
		ExpressionResult divide(const Value &other);


	private:
		void concatValueRange(const Value &other);
		TextRange valueRange;
		std::string value;
		ValueType type;
};
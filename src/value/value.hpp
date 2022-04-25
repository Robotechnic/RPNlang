#pragma once

class ExpressionResult;
class RPNFunction;

#include <string>
#include <stdexcept>
#include <tuple>
#include <functional>
#include <variant>
#include <cmath>
#include "tokens/tokentypes.hpp"
#include "tokens/token.hpp"
#include "textutilities/textrange.hpp"
#include "value/valuetype.hpp"
#include "rpnfunctions/rpnfunction.hpp"

typedef std::variant<int, float, bool, std::string, RPNFunction *> ValueStorage;

class Value {
	public:
		Value();
		Value(const Value &other);
		Value(std::string value, ValueType type, int line, int column);
		Value(std::string value, TokenType type, int line, int column);
		Value(std::string, int line, int column);
		Value(int value, int line, int column);
		Value(float value, int line, int column);
		Value(bool value, int line, int column);
		Value(RPNFunction * function, int line, int column);

		ValueType getType() const;

		float getFloatValue() const;
		int getIntValue() const;
		bool getBoolValue() const;
		std::string getStringValue() const;
		const RPNFunction * getFunctionValue() const;
		bool isCastableTo(ValueType type) const;
		bool isNumber() const;


		void setValue(std::string value);
		void setValue(float value);
		void setValue(int value);
		void setValue(bool value);

		ValueStorage getValue() const;

		TextRange getRange() const;

		static std::string stringType(const ValueType type);
		static ValueType valueType(const std::string type); 

		ExpressionResult setVariable(std::map<std::string, Value> &variables);

		ExpressionResult applyOperator(const Value &other, const Token &operatorToken, std::map<std::string, Value> &variables);

	private:
		void concatValueRange(const TextRange &otherRange);
		void concatValueRange(const Value &other);
		void concatValueRange(const Token &other);
		ExpressionResult opadd(const Value &other);
		ExpressionResult opsub(const Value &other);
		ExpressionResult opmul(const Value &other);
		ExpressionResult opdiv(const Value &other);
		ExpressionResult opmod(const Value &other);
		ExpressionResult oppow(const Value &other);
		ExpressionResult opgt(const Value &other);
		ExpressionResult opge(const Value &other);
		ExpressionResult oplt(const Value &other);
		ExpressionResult ople(const Value &other);
		ExpressionResult opne(const Value &other);
		ExpressionResult opeq(const Value &other);

		ValueStorage value;
		TextRange valueRange;
		ValueType type;
};


std::ostream &operator<<(std::ostream &os, const Value &value);
namespace std {
	std::string to_string(const Value &value);
};
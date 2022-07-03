#pragma once

class ExpressionResult;
class Token;
class Module;

#include <string>
#include <stdexcept>
#include <tuple>
#include <functional>
#include <variant>
#include <cmath>
#include "tokens/tokentypes.hpp"
#include "tokens/token.hpp"
#include "textutilities/textrange.hpp"
#include "rpnfunctions/rpnfunction.hpp"
#include "value/valuetype.hpp"
#include "context/context.hpp"

class RPNFunction;
typedef std::variant<int, float, bool, std::string, RPNFunction *> ValueStorage;


class Value {
	public:
		Value();
		Value(const Value &other, const Context *context);
		Value(std::string value, ValueType type, int line, int column);
		Value(std::string value, TokenType type, int line, int column);
		Value(std::string value, int line, int column);
		Value(std::string value, TextRange range);

		Value(int value, int line, int column);
		Value(float value, int line, int column);
		Value(bool value, int line, int column);
		Value(RPNFunction * function, int line, int column);

		Value(int value, TextRange range);
		Value(float value, TextRange range);
		Value(bool value, TextRange range);
		Value(RPNFunction * function, TextRange range);

		void clean();

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
		void setValue(RPNFunction * function);

		Value& to(ValueType type);

		ValueStorage getValue() const;

		TextRange getRange() const;

		static std::string stringType(const ValueType type);
		static ValueType valueType(const std::string type); 

		ExpressionResult getPathValue(const Context *context);
		ExpressionResult getVariableValue(const Context *context);

		ExpressionResult applyOperator(const Value &other, const Token &operatorToken, const Context *context);


		// some c++ operators which throw exceptions if the types are not compatibles
		// also only works with numbers for now
		Value operator+(const Value &other);
		Value operator-(const Value &other);
		Value operator-();
		Value operator*(const Value &other);
		Value operator/(const Value &other);
		bool operator>(const Value &other);
		bool operator>=(const Value &other);
		bool operator<(const Value &other);
		bool operator<=(const Value &other);
		bool operator!=(const Value &other);
		bool operator==(const Value &other);

		void concatValueRange(const Value &other);
		void concatValueRange(const Token &other);

	private:
		ExpressionResult opadd(const Value &other, const Context *context);
		ExpressionResult opsub(const Value &other, const Context *context);
		ExpressionResult opmul(const Value &other, const Context *context);
		ExpressionResult opdiv(const Value &other, const Context *context);
		ExpressionResult opmod(const Value &other, const Context *context);
		ExpressionResult oppow(const Value &other, const Context *context);
		ExpressionResult opgt(const Value &other, const Context *context);
		ExpressionResult opge(const Value &other, const Context *context);
		ExpressionResult oplt(const Value &other, const Context *context);
		ExpressionResult ople(const Value &other, const Context *context);
		ExpressionResult opne(const Value &other, const Context *context);
		ExpressionResult opeq(const Value &other, const Context *context);

		ValueStorage value;
		TextRange valueRange;
		ValueType type;
};


Value &operator+=(Value &lhs, const Value &rhs);
Value &operator-=(Value &lhs, const Value &rhs);
Value &operator*=(Value &lhs, const Value &rhs);
Value &operator/=(Value &lhs, const Value &rhs);

std::ostream &operator<<(std::ostream &os, const Value &value);
namespace std {
	std::string to_string(const Value &value);
};

#include "modules/module.hpp"
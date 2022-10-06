#pragma once

class ExpressionResult;
class Context;

#include <string>
#include <stdexcept>
#include <tuple>
#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"
#include "value/valuetypes.hpp"
#include "textutilities/textrange.hpp"
#include "context/context.hpp"

class Value;
typedef std::tuple<ExpressionResult, Value*> operatorResult;

class Value {
	public:
		Value(ValueType type, const TextRange range);
		virtual ~Value();

		virtual void clean() = 0;
		
		virtual bool isCastableTo(ValueType type) const = 0;
		virtual bool isNumber() const = 0;

		virtual Value *to(ValueType type);
		virtual Value *copy() const = 0;
		
		virtual std::string getStringValue() const = 0;

		static Value *empty();

		static std::string stringType(const ValueType type);
		static ValueType valueType(const std::string type); 

		TextRange getRange() const;
		ValueType getType() const;

		void concatValueRange(const Value *other);
		void concatValueRange(const Token *other);

		operatorResult applyOperator(const Value *other, const Token *operatorToken, const Context *context);
	
		virtual operatorResult opadd(const Value *other, const Context *context) = 0;
		virtual operatorResult opsub(const Value *other, const Context *context) = 0;
		virtual operatorResult opmul(const Value *other, const Context *context) = 0;
		virtual operatorResult opdiv(const Value *other, const Context *context) = 0;
		virtual operatorResult opmod(const Value *other, const Context *context) = 0;
		virtual operatorResult oppow(const Value *other, const Context *context) = 0;
		
		virtual operatorResult opgt(const Value *other, const Context *context) = 0;
		virtual operatorResult opge(const Value *other, const Context *context) = 0;
		virtual operatorResult oplt(const Value *other, const Context *context) = 0;
		virtual operatorResult ople(const Value *other, const Context *context) = 0;
		virtual operatorResult opne(const Value *other, const Context *context) = 0;
		virtual operatorResult opeq(const Value *other, const Context *context) = 0;

	protected:
		TextRange range;
		ValueType type;
};


std::ostream &operator<<(std::ostream &os, const Value *value);
namespace std {
	std::string to_string(const Value *value);
};
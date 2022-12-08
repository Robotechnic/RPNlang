#pragma once

class ExpressionResult;
class Value;

#include <string>
#include <string_view>
#include <stdexcept>
#include <tuple>
#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"
#include "value/valuetypes.hpp"
#include "textutilities/textrange.hpp"
#include "context/typedef.hpp"

typedef std::tuple<ExpressionResult, Value*> operatorResult;

class Value {
	public:
		Value(ValueType type, const TextRange range, bool interpreterValue);
		virtual ~Value() = default;
		
		virtual bool isCastableTo(ValueType type) const = 0;
		virtual bool isNumber() const = 0;

		virtual Value *to(ValueType type, bool interpreterValue = true);
		inline virtual Value *copy(bool interpreterValue = true) const = 0;
		
		inline virtual std::string getStringValue() const = 0;

		static Value *empty();

		static std::string stringType(const ValueType &type);
		static ValueType valueType(std::string_view type); 

		TextRange getRange() const;
		ValueType getType() const;
		std::string getStringType() const;

		void concatValueRange(const Value *other);
		void concatValueRange(const Token *other);

		/**
		 * @brief this member allow to check if the owner of this value is an interpreter instance
		 * This member allow to have values used multiples times, to store them in variables wihout need to copy them
		 * so if the owner is not an interpreter instance, the interpreter won't delete the value and the owner will manage
		 * it's deletion
		 */
		bool interpreterValue = false;

		operatorResult applyOperator(const Value *other, const Token *operatorToken, const ContextPtr &context);
	
		virtual operatorResult opadd(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opsub(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opmul(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opdiv(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opmod(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult oppow(const Value *other, const ContextPtr &context) const = 0;
		
		virtual operatorResult opgt(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opge(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult oplt(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult ople(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opne(const Value *other, const ContextPtr &context) const = 0;
		virtual operatorResult opeq(const Value *other, const ContextPtr &context) const = 0;

		static void deleteValue(Value **val);

	protected:
		TextRange range;
		ValueType type;
};


std::ostream &operator<<(std::ostream &os, const Value *value);
namespace std {
	std::string to_string(const Value *value);
};

#include "expressionresult/expressionresult.hpp"
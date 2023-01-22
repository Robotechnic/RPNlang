#pragma once

class ExpressionResult;
class Value;

#include <string>
#include <string_view>
#include <stdexcept>
#include <tuple>
#include "tokens/token.hpp"
#include "tokens/tokens/operatortoken.hpp"
#include "tokens/tokentypes.hpp"
#include "value/valuetypes.hpp"
#include "textutilities/textrange.hpp"
#include "context/typedef.hpp"


class Value {
	public:
		enum ValueOwner {
			INTERPRETER      = 0,
			PARENT_FUNCTION  = 1,
			CONTEXT_VARIABLE = 2,
			OBJECT_VALUE     = 3,
			VALUE_TOKEN      = 4,
			MODULE           = 5,
			EMPTY_VALUE      = 6
		};
		Value(ValueType type, const TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		virtual ~Value() = default;
		
		static bool isCastableTo(ValueType from, ValueType to);
		virtual bool isNumber() const = 0;

		virtual Value *to(ValueType type, ValueOwner owner = INTERPRETER) const = 0;
		inline virtual Value *copy(ValueOwner owner = INTERPRETER) const = 0;
		
		inline virtual std::string getStringValue() const = 0;

		static Value *empty();

		static std::string stringType(const ValueType &type);
		static ValueType valueType(std::string_view type); 

		TextRange getRange() const;
		TextRange getVariableRange() const;
		void setVariableRange(const TextRange &range);
		void setVariableRange(const TextRange &&range);
		ValueType getType() const;
		void setType(ValueType type);
		std::string getStringType() const;

		void concatValueRange(const Value *other);
		void concatValueRange(const Token *other);

		ValueOwner getOwner() const;
		void setOwner(ValueOwner owner, bool overwrite = false);

		Value *applyOperator(const Value *other, const Token *operatorToken, const ContextPtr &context);
	
		virtual Value *opadd(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opsub(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opmul(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opmod(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *oppow(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		
		virtual Value *opgt(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opge(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *oplt(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *ople(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opne(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;
		virtual Value *opeq(const Value *other, const TextRange &range, const ContextPtr &context) const = 0;

		static void deleteValue(Value **val, ValueOwner deleter);

	protected:
		TextRange range, variableRange;
		ValueType type;
		/**
		 * @brief this member allow to check if the owner of this value is an interpreter instance
		 * This member allow to have values used multiples times, to store them in variables wihout need to copy them
		 * so if the owner is not an interpreter instance, the interpreter won't delete the value and the owner will manage
		 * it's deletion
		 */
		ValueOwner owner;
};


std::ostream &operator<<(std::ostream &os, const Value *value);
namespace std {
	std::string to_string(const Value *value);
};

#include "expressionresult/expressionresult.hpp"
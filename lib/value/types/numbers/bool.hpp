#pragma once

#include <string>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"

class Int;
#include "value/types/numbers/int.hpp"

class Float;
#include "value/types/numbers/float.hpp"

class String;
#include "value/types/string.hpp"

class Bool : public Value {
	public:
		Bool(const std::string &value, TextRange range, bool interpreterValue);
		Bool(bool value, TextRange range, bool interpreterValue);

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type, bool interpreterValue = true);
		inline Value *copy(bool interpreterValue = true) const override;

		static Bool *empty() {
			return new Bool(false, TextRange(), true);
		}

		inline std::string getStringValue() const;

		bool getValue() const { return value; };

		operatorResult opadd(const Value *other, const ContextPtr &context) const override;
		operatorResult opsub(const Value *other, const ContextPtr &context) const override;
		operatorResult opmul(const Value *other, const ContextPtr &context) const override;
		operatorResult opdiv(const Value *other, const ContextPtr &context) const override;
		operatorResult opmod(const Value *other, const ContextPtr &context) const override;
		operatorResult oppow(const Value *other, const ContextPtr &context) const override;
		operatorResult opgt(const Value *other, const ContextPtr &context) const override;
		operatorResult opge(const Value *other, const ContextPtr &context) const override;
		operatorResult oplt(const Value *other, const ContextPtr &context) const override;
		operatorResult ople(const Value *other, const ContextPtr &context) const override;
		operatorResult opne(const Value *other, const ContextPtr &context) const override;
		operatorResult opeq(const Value *other, const ContextPtr &context) const override;

	private:
		bool value;
};
#pragma once

#include <string>
#include <cmath>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"

class Int;
#include "value/types/numbers/int.hpp"

class String;
#include "value/types/string.hpp"


class Float : public Value {
	public:
		Float(const std::string &value, TextRange range, bool interpreterValue);
		Float(float value, TextRange range, bool interpreterValue);

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type, bool interpreterValue = true);
		inline Value *copy(bool interpreterValue = true) const override;

		static Float *empty() {
			return new Float(0, TextRange(), true);
		}

		inline std::string getStringValue() const;

		float getValue() const { return value; };

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
		float value;
};
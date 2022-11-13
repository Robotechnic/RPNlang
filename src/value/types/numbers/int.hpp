#pragma once

#include <string>

#include "textutilities/textrange.hpp"

#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"

class Float;
#include "value/types/numbers/float.hpp"

class String;
#include "value/types/string.hpp"


class Int : public Value {
	public:
		Int(std::string value, TextRange range, bool interpreterValue);
		Int(int64_t value, TextRange range, bool interpreterValue);

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type, bool interpreterValue = true);
		inline Value *copy(bool interpreterValue = true) const override;

		static Int *empty() {
			return new Int(0, TextRange(), true);
		}

		std::string getStringValue() const;

		int64_t getValue() const { return value; };

		operatorResult opadd(const Value *other, const Context *context) const override;
		operatorResult opsub(const Value *other, const Context *context) const override;
		operatorResult opmul(const Value *other, const Context *context) const override;
		operatorResult opdiv(const Value *other, const Context *context) const override;
		operatorResult opmod(const Value *other, const Context *context) const override;
		operatorResult oppow(const Value *other, const Context *context) const override;
		operatorResult opgt(const Value *other, const Context *context) const override;
		operatorResult opge(const Value *other, const Context *context) const override;
		operatorResult oplt(const Value *other, const Context *context) const override;
		operatorResult ople(const Value *other, const Context *context) const override;
		operatorResult opne(const Value *other, const Context *context) const override;
		operatorResult opeq(const Value *other, const Context *context) const override;

	private:
		int64_t value;
};
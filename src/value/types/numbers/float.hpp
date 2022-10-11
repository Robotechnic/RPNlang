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
		Float(std::string value, TextRange range);
		Float(float value, TextRange range);

		void clean(){};

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type);
		inline Value *copy() const override;

		static Float *empty() {
			return new Float(0, TextRange());
		}

		std::string getStringValue() const;

		float getValue() const { return value; };

		operatorResult opadd(const Value *other, const Context *context) override;
		operatorResult opsub(const Value *other, const Context *context) override;
		operatorResult opmul(const Value *other, const Context *context) override;
		operatorResult opdiv(const Value *other, const Context *context) override;
		operatorResult opmod(const Value *other, const Context *context) override;
		operatorResult oppow(const Value *other, const Context *context) override;
		operatorResult opgt(const Value *other, const Context *context) override;
		operatorResult opge(const Value *other, const Context *context) override;
		operatorResult oplt(const Value *other, const Context *context) override;
		operatorResult ople(const Value *other, const Context *context) override;
		operatorResult opne(const Value *other, const Context *context) override;
		operatorResult opeq(const Value *other, const Context *context) override;

	private:
		float value;
};
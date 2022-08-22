#pragma once

#include <string>
#include <cmath>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"


class Bool;
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
		Value *copy() const override;
		std::string getStringValue() const;

		float getValue() const { return value; };

		operatorReturn opadd(const Value *other, const Context *context) override;
		operatorReturn opsub(const Value *other, const Context *context) override;
		operatorReturn opmul(const Value *other, const Context *context) override;
		operatorReturn opdiv(const Value *other, const Context *context) override;
		operatorReturn opmod(const Value *other, const Context *context) override;
		operatorReturn oppow(const Value *other, const Context *context) override;
		operatorReturn opgt(const Value *other, const Context *context) override;
		operatorReturn opge(const Value *other, const Context *context) override;
		operatorReturn oplt(const Value *other, const Context *context) override;
		operatorReturn ople(const Value *other, const Context *context) override;
		operatorReturn opne(const Value *other, const Context *context) override;
		operatorReturn opeq(const Value *other, const Context *context) override;

		static Float emptyFloat;

	private:
		float value;
};
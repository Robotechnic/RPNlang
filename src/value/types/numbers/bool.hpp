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
		Bool(std::string value, TextRange range);
		Bool(bool value, TextRange range);

		void clean(){};

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type);
		Value *copy() const override;

		static Bool *empty() {
			return new Bool(false, TextRange());
		}

		std::string getStringValue() const;

		bool getValue() const { return value; };

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
		bool value;
};
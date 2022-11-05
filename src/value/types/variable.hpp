#pragma once

#include <string>
#include "textutilities/textrange.hpp"
#include "value/value.hpp"

#include "value/types/numbers/bool.hpp"

class Variable : public Value {
	public:
		Variable(std::string value, TextRange range);

		std::string getStringValue() const;

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		inline Value *copy(bool interpreterValue = true) const override;

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
		std::string value;
};
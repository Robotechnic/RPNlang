#pragma once

#include "textutilities/textrange.hpp"
#include "value/value.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/rpnfunction.hpp"
#include "value/types/numbers/bool.hpp"

class Function : public Value {
	public:
		Function(const RPNFunction* function, TextRange range, bool interpreterValue);

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		inline Value *copy(bool interpreterValue = true) const override;

		std::string getStringValue() const;

		const RPNFunction* getValue() const;

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
		const RPNFunction *function;
};
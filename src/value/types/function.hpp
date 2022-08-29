#pragma once

#include <memory>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "value/types/numbers/bool.hpp"

class Function : public Value {
	public:
		Function(RPNFunction* function, TextRange range);
		Function(std::shared_ptr<RPNFunction> function, TextRange range);

		void clean();

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		Value *copy() const override;

		std::string getStringValue() const;

		std::shared_ptr<RPNFunction> getValue() const;

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
		std::shared_ptr<RPNFunction> function;
};
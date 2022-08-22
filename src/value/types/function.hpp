#pragma once

#include "textutilities/textrange.hpp"
#include "value/value.hpp"
#include "rpnfunctions/typedef.hpp"
#include "rpnfunctions/rpnfunction.hpp"
#include "rpnfunctions/builtinsrpnfunction.hpp"
#include "value/types/numbers/bool.hpp"

class Function : public Value {
	public:
		Function(RPNFunction* function, TextRange range);

		void clean();

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		Value *copy() const override;

		std::string getStringValue() const;

		RPNFunction* getValue() const { return function; };

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

	private:
		RPNFunction* function;
};
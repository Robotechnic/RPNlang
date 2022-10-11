#pragma once

#include <string>
#include "textutilities/textrange.hpp"
#include "value/value.hpp"

#include "value/types/numbers/bool.hpp"

class None : public Value {
	public:
		None(TextRange range);

		void clean(){};

		std::string getStringValue() const;

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		inline Value *copy() const override;

		static None *empty() {
			return new None(TextRange());
		}

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
};
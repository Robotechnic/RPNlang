#pragma once

#include <string>
#include "textutilities/textrange.hpp"
#include "value/value.hpp"

class Variable : public Value {
	public:
		Variable(ValueType type, std::string value, TextRange range);

		void clean(){};

		std::string getStringValue() const;

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		Value *to(ValueType type);
		Value *copy() const override;

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


		static Variable emptyVariable;
	private:
		std::string value;
};
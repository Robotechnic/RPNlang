#pragma once

#include <string>

#include "textutilities/textrange.hpp"

#include "value/value.hpp"

class Bool;
#include "value/types/numbers/bool.hpp"

class Float;
#include "value/types/numbers/float.hpp"

class String;
#include "value/types/string.hpp"


class Int : public Value {
	public:
		Int(std::string value, TextRange range);
		Int(int64_t value, TextRange range);

		void clean(){};

		bool isCastableTo(ValueType type) const override;
		bool isNumber() const { return true; };

		Value *to(ValueType type);
		Value *copy() const override;
		std::string getStringValue() const;

		int64_t getValue() const { return value; };
		

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

		static Int emptyInt;

	private:
		int64_t value;
};
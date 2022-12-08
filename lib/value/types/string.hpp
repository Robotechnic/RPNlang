#pragma once

#include <string>
#include "textutilities/textrange.hpp"
#include "value/value.hpp"

#include "value/types/numbers/bool.hpp"

class String : public Value {
	public:
		String(std::string_view value, TextRange range, bool interpreterValue);

		inline std::string getStringValue() const;

		bool isCastableTo(ValueType type) const;
		bool isNumber() const { return false; };

		static String *empty() {
			return new String("", TextRange(), true);
		}

		Value *to(ValueType type, bool interpreterValue = true);
		inline Value *copy(bool interpreterValue = true) const override;

		operatorResult opadd(const Value *other, const ContextPtr &context) const override;
		operatorResult opsub(const Value *other, const ContextPtr &context) const override;
		operatorResult opmul(const Value *other, const ContextPtr &context) const override;
		operatorResult opdiv(const Value *other, const ContextPtr &context) const override;
		operatorResult opmod(const Value *other, const ContextPtr &context) const override;
		operatorResult oppow(const Value *other, const ContextPtr &context) const override;
		operatorResult opgt(const Value *other, const ContextPtr &context) const override;
		operatorResult opge(const Value *other, const ContextPtr &context) const override;
		operatorResult oplt(const Value *other, const ContextPtr &context) const override;
		operatorResult ople(const Value *other, const ContextPtr &context) const override;
		operatorResult opne(const Value *other, const ContextPtr &context) const override;
		operatorResult opeq(const Value *other, const ContextPtr &context) const override;

	private:
		std::string value;
};
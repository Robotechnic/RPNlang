#pragma once

#include <string>
#include <cmath>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"

class Int;
#include "value/types/numbers/int.hpp"

class String;
#include "value/types/string.hpp"


class Float : public Value {
	public:
		Float(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		Float(float value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());

		bool isNumber() const { return true; };

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		inline Value *copy(ValueOwner owner = INTERPRETER) const override;

		static Float *empty() {
			return Float::emptyFloat.get();
		}

		inline std::string getStringValue() const;

		float getValue() const { return value; };

		Value *opadd(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opsub(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opmul(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opmod(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *oppow(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opgt(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opge(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *oplt(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *ople(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opne(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		Value *opeq(const Value *other, const TextRange &range, const ContextPtr &context) const override;

	private:
		float value;
		static std::unique_ptr<Float> emptyFloat;
};
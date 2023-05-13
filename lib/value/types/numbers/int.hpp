#pragma once

#include <string>

#include "textutilities/textrange.hpp"

#include "value/value.hpp"
#include "value/types/numbers/bool.hpp"

class Float;
#include "value/types/numbers/float.hpp"

class String;
#include "value/types/string.hpp"


class Int : public Value {
	public:
		Int(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		Int(int64_t value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		
		bool isNumber() const override { return true; };

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		Value *copy(ValueOwner owner = INTERPRETER) const override;

		static Int *empty() {
			return Int::emptyInt.get();
		}

		std::string getStringValue() const override;

		int64_t getValue() const { return value; };

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
		int64_t value;
		static std::unique_ptr<Int> emptyInt;
};
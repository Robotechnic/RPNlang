#pragma once

#include <string>

#include "textutilities/textrange.hpp"
#include "value/value.hpp"

class Int;
#include "value/types/numbers/int.hpp"

class Float;
#include "value/types/numbers/float.hpp"

class String;
#include "value/types/string.hpp"

class Bool : public Value {
	public:
		Bool(const std::string &value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		Bool(bool value, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());

		bool isNumber() const { return true; };

		Value *to(ValueType type, ValueOwner owner = INTERPRETER)const override;
		inline Value *copy(ValueOwner owner = INTERPRETER) const override;

		static Bool *empty() {
			return Bool::emptyBool.get();
		}

		inline std::string getStringValue() const;

		bool getValue() const { return value; };

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
		bool value;
		static std::unique_ptr<Bool> emptyBool;
};
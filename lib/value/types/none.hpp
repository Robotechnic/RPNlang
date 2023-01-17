#pragma once

#include <string>
#include "textutilities/textrange.hpp"
#include "value/value.hpp"

#include "value/types/numbers/bool.hpp"

class None : public Value {
	public:
		None(TextRange range, ValueOwner owner);

		inline std::string getStringValue() const;

		bool isNumber() const { return false; };

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		inline Value *copy(ValueOwner owner = INTERPRETER) const override;

		static None *empty() {
			return None::emptyNone.get();
		}

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
		static std::unique_ptr<None> emptyNone;
};
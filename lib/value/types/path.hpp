#pragma once

#include <string>
#include <vector>

#include "value/value.hpp"
#include "textutilities/textutilities.hpp"

class Path : public Value {
	public:
		Path(std::vector<std::string> path, TextRange range, ValueType type = PATH);
		
		bool isNumber() const override;

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		inline Value* copy(ValueOwner owner = INTERPRETER) const override;

		inline std::string getStringValue() const;
		std::vector<std::string> getPath() const;
		std::string_view at(size_t index) const;
		std::string ats(size_t index) const;
		size_t size() const;

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
		std::vector<std::string> path;
};
#pragma once

#include <string>
#include <vector>

#include "value/value.hpp"
#include "textutilities/textutilities.hpp"

class Path : public Value {
	public:
		Path(std::vector<std::string> path, TextRange range, ValueType type = PATH);
		
		bool isCastableTo(ValueType type) const override;
		bool isNumber() const override;

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		inline Value* copy(ValueOwner owner = INTERPRETER) const override;

		inline std::string getStringValue() const;
		std::vector<std::string> getPath() const;
		std::string_view at(size_t index) const;
		std::string ats(size_t index) const;
		size_t size() const;

		operatorResult opadd(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opsub(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opmul(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opmod(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult oppow(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opgt(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opge(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult oplt(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult ople(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opne(const Value *other, const TextRange &range, const ContextPtr &context) const override;
		operatorResult opeq(const Value *other, const TextRange &range, const ContextPtr &context) const override;
	
	private:
		std::vector<std::string> path;
};
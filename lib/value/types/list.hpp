#pragma once

#include <vector>
#include "value/value.hpp"
#include "value/types/string.hpp"
#include "value/types/numbers/bool.hpp"

class List : public Value {
	public:
		List(TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		List(std::vector<Value *> values, TextRange range, ValueOwner owner, const TextRange variableRange = TextRange());
		~List();

		bool isNumber() const override;
		std::vector<Value *> getValues() const;

		unsigned int size() const;
		Value *at(unsigned int index) const;
		void set(unsigned int index, Value *value);
		void push(Value *value);
		Value *pop();
		void insert(unsigned int index, Value *value);
		void remove(unsigned int index);
		void clear();

		List *empty() {
			return List::emptyList.get();
		}

		Value *to(ValueType type, ValueOwner owner = INTERPRETER) const override;
		inline Value *copy(ValueOwner owner = INTERPRETER) const override;
		
		inline std::string getStringValue() const override;

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
		static std::unique_ptr<List> emptyList;
		std::vector<Value *> values;
};
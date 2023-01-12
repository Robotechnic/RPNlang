#pragma once

#include <vector>
#include "value/value.hpp"
#include "value/types/string.hpp"
#include "value/types/numbers/bool.hpp"

class List : public Value {
	public:
		List(TextRange range, ValueOwner owner);
		List(std::vector<Value *> values, TextRange range, ValueOwner owner);
		~List();

		bool isCastableTo(ValueType type) const override;
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
		static std::unique_ptr<List> emptyList;
		std::vector<Value *> values;
};
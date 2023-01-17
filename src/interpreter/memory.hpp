#pragma once

#include <stack>
#include <stdexcept>
#include "value/value.hpp"
#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"

class Memory {
	public:
		Memory();
		~Memory();

		void push(Value* value);
		Value*& pop();
		Value*& popVariableValue(const ContextPtr &context);
		Value*& top();
		void clear(size_t offset = 0);
		bool empty();
		size_t size();
		ExpressionResult sizeExpected(size_t size, const std::string &message, TextRange range, const ContextPtr &ctx);

	private:
		std::stack<Value*> stack;
};

#include "modules/module.hpp"

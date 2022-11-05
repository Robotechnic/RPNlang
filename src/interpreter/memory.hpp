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
		ExpressionResult topVariableToValue(const Context *context);
		Value*& pop();
		ExpressionResult popVariableValue(Value *&value, const Context *context);
		Value*& top();
		void clear(unsigned long int offset = 0);
		bool empty();
		unsigned long int size();
		ExpressionResult sizeExpected(unsigned long int size, std::string message, TextRange range, const Context *ctx);

	private:
		std::stack<Value*> stack;
};
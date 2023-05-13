#pragma once

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include <stack>
#include <stdexcept>

class Memory {
  public:
	Memory();
	~Memory();

	void push(Value *const &value);
	Value *&pop();
	Value *&popVariableValue(const ContextPtr &context);
	Value *&top();
	void clear(size_t offset = 0);
	bool empty();
	size_t size();
	ExpressionResult sizeExpected(size_t size, const std::string &message, TextRange range,
								  const ContextPtr &ctx);

  private:
	Value *&getStructureValue(Value *pathValue, const ContextPtr &context);
	std::stack<Value *> stack;
};

#include "modules/module.hpp"

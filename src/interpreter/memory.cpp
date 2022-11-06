#include "interpreter/memory.hpp"

Memory::Memory() : stack() {}
Memory::~Memory() {
	this->clear();
}

void Memory::push(Value* value) {
	this->stack.push(value);
}

Value*& Memory::pop() {
	if (this->stack.empty())
		throw std::runtime_error("Memory stack is empty");
	Value*& value = this->stack.top();
	this->stack.pop();
	return value;
}

ExpressionResult Memory::popVariableValue(Value *&value, const Context *context) {
	if (this->stack.top()->getType() != VARIABLE) {
		value = this->pop();
		return ExpressionResult();
	}
	
	ExpressionResult result = context->getValue(this->stack.top(), value);
	if (result.error()) return result;
	Value::deleteValue(&this->stack.top());
	this->stack.pop();
	return result;
}

Value*& Memory::top() {
	return this->stack.top();
}

void Memory::clear(unsigned long int offset) {
	while (this->stack.size() > offset) {
		Value::deleteValue(&this->stack.top());
		this->stack.pop();
	}
}

bool Memory::empty() {
	return this->stack.empty();
}

unsigned long int Memory::size() {
	return this->stack.size();
}

/**
 * @brief check if the stack size is the expected one
 * 
 * @param size the minimum size of the stack
 * @param message the error message to display in case of error
 * @param range the default range in case where the stack is empty
 * @param ctx the current context
 * @return ExpressionResult if the stack size is correct or not
 */
ExpressionResult Memory::sizeExpected(unsigned long int size, std::string message, TextRange range, const Context *ctx) {
	if (this->stack.size() == 0 && size != 0) 
		return ExpressionResult(message + " (Memory is empty)", range, ctx);
	if (this->stack.size() < size) {
		TextRange firstRange = this->stack.top()->getRange();
		this->clear(1);
		return ExpressionResult(
			message,
			this->stack.top()->getRange().merge(firstRange),
			ctx
		);
	}
	return ExpressionResult();
}

/**
 * @brief replace top value with it's value if it's a variable
 * 
 * @param context the current context
 * @return ExpressionResult the result of the operation
 */
ExpressionResult Memory::topVariableToValue(const Context *context) {
	if (this->stack.size() == 0) 
		throw std::runtime_error("Memory::topVariableToValue: stack is empty");
	if (this->stack.top()->getType() != VARIABLE) return ExpressionResult();

	Value *value;
	ExpressionResult result = context->getValue(this->stack.top(), value);
	Value::deleteValue(&this->stack.top());
	this->stack.pop();
	if (result.error()) return result;
	this->stack.push(value);
	return ExpressionResult();
}
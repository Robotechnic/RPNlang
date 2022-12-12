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

ExpressionResult Memory::popVariableValue(Value *&value, const ContextPtr &context) {
	if (this->stack.top()->getType() != VARIABLE && this->stack.top()->getType() != PATH) {
		value = this->pop();
		return ExpressionResult();
	}
	
	ExpressionResult result;
	if (this->stack.top()->getType() == VARIABLE)
		result = context->getValue(this->stack.top(), value);
	else
		result = Module::getModuleValue(this->stack.top(), value, context);
	Value::deleteValue(&this->stack.top(), Value::INTERPRETER);
	this->stack.pop();
	if (result.error()) return result;
	return result;
}

Value*& Memory::top() {
	return this->stack.top();
}

void Memory::clear(size_t offset) {
	while (this->stack.size() > offset) {
		Value::deleteValue(&this->stack.top(), Value::INTERPRETER);
		this->stack.pop();
	}
}

bool Memory::empty() {
	return this->stack.empty();
}

size_t Memory::size() {
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
ExpressionResult Memory::sizeExpected(size_t size, const std::string &message, TextRange range, const ContextPtr &ctx) {
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
ExpressionResult Memory::topVariableToValue(const ContextPtr &context) {
	if (this->stack.size() == 0) 
		throw std::runtime_error("Memory::topVariableToValue: stack is empty");
	if (this->stack.top()->getType() != VARIABLE && this->stack.top()->getType() != PATH) 
		return ExpressionResult();

	Value *value;
	ExpressionResult result;
	if (this->stack.top()->getType() == VARIABLE)
		result = context->getValue(this->stack.top(), value);
	else
		result = Module::getModuleValue(this->stack.top(), value, context);
	
	Value::deleteValue(&this->stack.top(), Value::INTERPRETER);
	this->stack.pop();
	if (result.error()) return result;
	this->stack.push(value);
	return ExpressionResult();
}
#include "interpreter/memory.hpp"

Memory::Memory() : stack() {}
Memory::~Memory() {
	this->clear();
}

void Memory::push(Value* value) {
	this->stack.push(value);
}

Value* Memory::pop() {
	Value* value = this->stack.top();
	this->stack.pop();
	return value;
}

Value* Memory::top() {
	return this->stack.top();
}

void Memory::clear(unsigned long int offset) {
	while (this->stack.size() > offset) {
		delete this->stack.top();
		this->stack.pop();
	}
}

bool Memory::empty() {
	return this->stack.empty();
}

unsigned long int Memory::size() {
	return this->stack.size();
}

ExpressionResult Memory::sizeExpected(unsigned long int size, std::string message, TextRange range, const Context *ctx) {
	if (this->stack.size() == 0 && size != 0) 
		return ExpressionResult(message + "(Memory is empty)", range, ctx);
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
#include "interpreter/memory.hpp"

Memory::Memory() : stack() {}
Memory::~Memory() {
	this->clear();
}

void Memory::push(Value* const &value) {
	this->stack.push(value);
}

Value*& Memory::pop() {
	if (this->stack.empty())
		throw std::runtime_error("Memory stack is empty");
	Value*& value = this->stack.top();
	this->stack.pop();
	return value;
}

Value *& Memory::popVariableValue(const ContextPtr &context) {
	if (
		this->stack.top()->getType() != VARIABLE && 
		this->stack.top()->getType() != PATH &&
		this->stack.top()->getType() != BUILTIN_PATH &&
		this->stack.top()->getType() != STRUCT_ACCESS &&
		this->stack.top()->getType() != LIST_ELEMENT) {
		return this->pop();
	}
	
	ExpressionResult result;
	Value **value;
	switch (this->stack.top()->getType()) {
		case VARIABLE:
			value = &context->getValue(this->stack.top());
			break;
		case PATH:
		case BUILTIN_PATH:
			value = &Module::getModuleValue(this->stack.top());
			break;
		case STRUCT_ACCESS:
			value = &static_cast<Struct*>(
				Struct::getStruct(static_cast<Path*>(this->stack.top()), context)
			)->getMember(static_cast<Path*>(this->stack.top()));
			break;
		case LIST_ELEMENT:
			value = &static_cast<ListElement*>(this->stack.top())->get();
			break;
		default:
			throw std::runtime_error("This should not happen");
	}
	TextRange range = this->stack.top()->getRange();
	Value::deleteValue(&this->stack.top(), Value::INTERPRETER);
	this->stack.pop();
	(*value)->setVariableRange(range);
	return *value;
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

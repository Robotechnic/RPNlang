#include "interpreter/memory.hpp"

Memory::Memory() = default;
Memory::~Memory() {
	this->clear();
}

void Memory::push(Value *const &value) {
	this->stack.push(value);
}

Value *&Memory::pop() {
	if (this->stack.empty()) {
		throw std::runtime_error("Memory stack is empty");
	}
	Value *&value = this->stack.top();
	this->stack.pop();
	return value;
}

Value *&Memory::popVariableValue(const ContextPtr &context) {
	ExpressionResult const result;
	Value **value = nullptr;
	Value **name = &this->stack.top();
	this->stack.pop();
	switch ((*name)->getType()) {
		case VARIABLE:
			value = &context->getValue(*name);
			break;
		case PATH:
		case BUILTIN_PATH:
			value = &Module::getModuleValue(*name);
			break;
		case STRUCT_ACCESS:
			value = &this->getStructureValue(*name, context);
			break;
		case LIST_ELEMENT:
			value = &dynamic_cast<ListElement *>(*name)->get();
			break;
		default:
			return *name;
	}
	TextRange const range = (*name)->getRange();
	Value::deleteValue(name, Value::INTERPRETER);
	(*value)->setVariableRange(range);
	return *value;
}

Value *&Memory::getStructureValue(Value *pathValue, const ContextPtr &context) {
	Path const *path = dynamic_cast<Path *>(pathValue);
	if (this->stack.top()->getType() == VARIABLE) {
		Value const *name = this->pop();
		return dynamic_cast<Struct *>(Struct::getStruct(name, path, context))->getMember(path);
	}
	Value *top = this->popVariableValue(context);
	return dynamic_cast<Struct *>(top)->getMember(path);
}

Value *&Memory::top() {
	return this->stack.top();
}

void Memory::clear(size_t offset) {
	while (this->stack.size() > offset) {
		Value::deleteValue(&this->stack.top(), Value::INTERPRETER);
		this->stack.pop();
	}
}

bool Memory::empty() const {
	return this->stack.empty();
}

size_t Memory::size() const {
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
ExpressionResult Memory::sizeExpected(size_t size, const std::string &message, TextRange range,
									  const ContextPtr &ctx) {
	if (this->stack.empty() && size != 0) {
		return {message + " (Memory is empty)", range, ctx};
	}
	if (this->stack.size() < size) {
		TextRange const firstRange = this->stack.top()->getRange();
		this->clear(1);
		return {message, this->stack.top()->getRange().merge(firstRange), ctx};
	}
	return {};
}

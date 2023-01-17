#include "value/types/none.hpp"

None::None(TextRange range, ValueOwner owner) : Value(NONE, range, owner) {}

std::unique_ptr<None> None::emptyNone = std::make_unique<None>(TextRange(), Value::EMPTY_VALUE);

inline std::string None::getStringValue() const {
	return "NONE";
}

Value *None::to(ValueType type, ValueOwner owner) const {
	if (type == NONE || type == ANY)
		return new None(this->range, owner);
	if (type == BOOL)
		return new Bool(false, this->range, owner);
	
	throw std::runtime_error("Invalid value type");
}

 Value *None::copy(ValueOwner owner) const {
	return new None(range, owner);
}

Value *None::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot add value of type " + other->getStringType() + " to NONE");
}

Value *None::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot substract value of type " + other->getStringType() + " from NONE");
}

Value *None::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot multiply value of type NONE by value of type " + other->getStringType());
}

Value *None::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot divide value of type NONE by value of type " + other->getStringType());
}

Value *None::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot mod value of type NONE by value of type " + other->getStringType());
}

Value *None::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot power value of type NONE by value of type " + other->getStringType());
}

Value *None::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type NONE with value of type " + other->getStringType());		
}

Value *None::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type NONE with value of type " + other->getStringType());		
}

Value *None::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type NONE with value of type " + other->getStringType());			
}

Value *None::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type NONE with value of type " + other->getStringType());					
}

Value *None::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return new Bool(other->getType() != NONE, other->getRange(), Value::INTERPRETER);
}

Value *None::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return new Bool(other->getType() == NONE, other->getRange(), Value::INTERPRETER);
}

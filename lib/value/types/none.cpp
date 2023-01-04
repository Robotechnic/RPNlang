#include "value/types/none.hpp"

None::None(TextRange range, ValueOwner owner) : Value(NONE, range, owner) {}

std::unique_ptr<None> None::emptyNone = std::make_unique<None>(TextRange(), Value::EMPTY_VALUE);

inline std::string None::getStringValue() const {
	return "NONE";
}

bool None::isCastableTo(ValueType type) const {
	return type == NONE || type == ANY || type == BOOL;
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

operatorResult None::opadd(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot add value of type " + other->getStringType() + " to NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opsub(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot subtract value of type " + other->getStringType() + " from NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opmul(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot multiply value of type " + other->getStringType() + " with NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opdiv(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot divide None value by value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorResult None::opmod(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot modulo None value by value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::oppow(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot power None value by value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorResult None::opgt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare None value with value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);		
}

operatorResult None::opge(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare None value with value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);			
}

operatorResult None::oplt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare None value with value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);				
}

operatorResult None::ople(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare None value with value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);						
}

operatorResult None::opne(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(),
		new Bool(other->getType() != NONE, other->getRange(), Value::INTERPRETER)
	);
}

operatorResult None::opeq(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(),
		new Bool(other->getType() == NONE, other->getRange(), Value::INTERPRETER)
	);
}

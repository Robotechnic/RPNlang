#include "value/types/none.hpp"

None::None(TextRange range, bool interpreterValue) : Value(NONE, range, interpreterValue) {}

std::string None::getStringValue() const {
	return "NONE";
}

bool None::isCastableTo(ValueType type) const {
	return type == NONE;
}

Value *None::to(ValueType type, bool interpreterValue) {
	if (type == NONE)
		return new None(this->range, interpreterValue);
	if (type == BOOL)
		return new Bool(false, this->range, interpreterValue);
	
	throw std::runtime_error("Invalid value type");
}

 Value *None::copy(bool interpreterValue) const {
	return new None(range, interpreterValue);
}

operatorResult None::opadd(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot add value of type " + Value::stringType(other->getType()) + " to NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opsub(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot subtract value of type " + Value::stringType(other->getType()) + " from NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opmul(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot multiply value of type " + Value::stringType(other->getType()) + " with NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::opdiv(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot divide None value by value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorResult None::opmod(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot modulo None value by value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult None::oppow(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot power None value by value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorResult None::opgt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);		
}

operatorResult None::opge(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);			
}

operatorResult None::oplt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);				
}

operatorResult None::ople(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);						
}

operatorResult None::opne(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(),
		new Bool(other->getType() != NONE, other->getRange(), true)
	);
}

operatorResult None::opeq(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(),
		new Bool(other->getType() == NONE, other->getRange(), true)
	);
}

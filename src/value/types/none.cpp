#include "value/types/none.hpp"

None::None(TextRange range) : Value(NONE, range) {}

std::string None::getStringValue() const {
	return "";
}

bool None::isCastableTo(ValueType type) const {
	return type == NONE;
}

Value *None::to(ValueType type) {
	if (type == NONE)
		return this;
	
	throw std::runtime_error("Invalid value type");
}

Value *None::copy() const {
	return new None(range);
}

operatorReturn None::opadd(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot add value of type" + Value::stringType(other->getType()) + " to NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorReturn None::opsub(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot subtract value of type" + Value::stringType(other->getType()) + " from NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorReturn None::opmul(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot multiply value of type" + Value::stringType(other->getType()) + " with NONE",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorReturn None::opdiv(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot divide None value by value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorReturn None::opmod(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot modulo None value by value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorReturn None::oppow(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot power None value by value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);	
}

operatorReturn None::opgt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);		
}

operatorReturn None::opge(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);			
}

operatorReturn None::oplt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);				
}

operatorReturn None::ople(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare None value with value of type" + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);						
}

operatorReturn None::opne(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(),
		new Bool(other->getType() != NONE, other->getRange())
	);
}

operatorReturn None::opeq(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(),
		new Bool(other->getType() == NONE, other->getRange())
	);
}


None None::emptyNone = None(TextRange());
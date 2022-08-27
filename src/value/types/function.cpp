#include "value/types/function.hpp"

Function::Function(RPNFunction* function, TextRange range) :
	Value(FUNCTION, range),
	function(function) {}

void Function::clean() {
	delete function;
}

bool Function::isCastableTo(ValueType type) const {
	return type == FUNCTION || type == STRING;
}

Value *Function::to(ValueType type) {
	switch (type) {
		case FUNCTION:
			return this;
		case STRING:
			return new String(function->getName(), range);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Function::copy() const {
	return new Function(function, range);
}

std::string Function::getStringValue() const {
	if (function->getName() == "") {
		return "<anonymous function>";
	}

	return "<function " + function->getName() + ">";
}


operatorResult Function::opadd(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot add value of type " + Value::stringType(other->getType()) + " to a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opsub(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot subtract value of type " + Value::stringType(other->getType()) + " from a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmul(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot multiply a function by a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opdiv(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot divide a function by a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmod(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot modulo a function by a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oppow(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot exponentiate a function by a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opgt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opge(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oplt(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::ople(const Value *other, const Context *context) {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opne(const Value *other, const Context *context) {
	if (other->getType() != FUNCTION)
		return std::make_tuple(
			ExpressionResult(),
			new Bool(false, other->getRange())
		);

	return std::make_tuple(
		ExpressionResult(),
		new Bool(function != static_cast<const Function *>(other)->getValue(), other->getRange())
	);
}

operatorResult Function::opeq(const Value *other, const Context *context) {
	if (other->getType() != FUNCTION)
		return std::make_tuple(
			ExpressionResult(),
			new Bool(false, other->getRange())
		);

	return std::make_tuple(
		ExpressionResult(),
		new Bool(function == static_cast<const Function *>(other)->getValue(), other->getRange())
	);
}

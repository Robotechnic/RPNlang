#include "value/types/function.hpp"

Function::Function(const RPNFunction* function, TextRange range, ValueOwner owner) :
	Value(FUNCTION, range, owner),
	function(function) {}

bool Function::isCastableTo(ValueType type) const {
	return type == FUNCTION || type == STRING;
}

Value *Function::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case FUNCTION:
			return new Function(this->function, this->range, owner);
		case STRING:
			return new String(function->getName(), range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Function::copy(ValueOwner owner) const {
	return new Function(function, range, owner);
}

inline std::string Function::getStringValue() const {
	if (function->getName() == "") {
		return "<anonymous function>";
	}

	return "<function " + function->getName() + ">";
}

const RPNFunction* Function::getValue() const {
	return this->function;
}	


operatorResult Function::opadd(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot add value of type " + other->getStringType() + " to a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opsub(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot subtract value of type " + other->getStringType() + " from a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmul(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot multiply a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opdiv(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot divide a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmod(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot modulo a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oppow(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot exponentiate a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opgt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opge(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oplt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::ople(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opne(const Value *other, const ContextPtr &context) const {
	if (other->getType() != FUNCTION)
		return std::make_pair(
			ExpressionResult(),
			new Bool(false, other->getRange(), Value::INTERPRETER)
		);

	return std::make_pair(
		ExpressionResult(),
		new Bool(function != static_cast<const Function *>(other)->getValue(), other->getRange(), Value::INTERPRETER)
	);
}

operatorResult Function::opeq(const Value *other, const ContextPtr &context) const {
	if (other->getType() != FUNCTION)
		return std::make_pair(
			ExpressionResult(),
			new Bool(false, other->getRange(), Value::INTERPRETER)
		);

	return std::make_pair(
		ExpressionResult(),
		new Bool(function == static_cast<const Function *>(other)->getValue(), other->getRange(), Value::INTERPRETER)
	);
}

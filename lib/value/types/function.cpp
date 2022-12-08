#include "value/types/function.hpp"

Function::Function(const RPNFunction* function, TextRange range, bool interpreterValue) :
	Value(FUNCTION, range, interpreterValue),
	function(function) {}

bool Function::isCastableTo(ValueType type) const {
	return type == FUNCTION || type == STRING;
}

Value *Function::to(ValueType type, bool interpreterValue) {
	switch (type) {
		case FUNCTION:
			return new Function(this->function, this->range, interpreterValue);
		case STRING:
			return new String(function->getName(), range, interpreterValue);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Function::copy(bool interpreterValue) const {
	return new Function(function, range, interpreterValue);
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
	return std::make_tuple(
		ExpressionResult(
			"Cannot add value of type " + other->getStringType() + " to a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opsub(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot subtract value of type " + other->getStringType() + " from a function",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmul(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot multiply a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opdiv(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot divide a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opmod(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot modulo a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oppow(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot exponentiate a function by a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opgt(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::opge(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::oplt(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare a function to a value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult Function::ople(const Value *other, const ContextPtr &context) const {
	return std::make_tuple(
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
		return std::make_tuple(
			ExpressionResult(),
			new Bool(false, other->getRange(), true)
		);

	return std::make_tuple(
		ExpressionResult(),
		new Bool(function != static_cast<const Function *>(other)->getValue(), other->getRange(), true)
	);
}

operatorResult Function::opeq(const Value *other, const ContextPtr &context) const {
	if (other->getType() != FUNCTION)
		return std::make_tuple(
			ExpressionResult(),
			new Bool(false, other->getRange(), true)
		);

	return std::make_tuple(
		ExpressionResult(),
		new Bool(function == static_cast<const Function *>(other)->getValue(), other->getRange(), true)
	);
}

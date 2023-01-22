#include "value/types/function.hpp"

Function::Function(const RPNFunction* function, TextRange range, ValueOwner owner, const TextRange variableRange) :
	Value(FUNCTION, range, owner, variableRange),
	function(function) {}


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
	return new Function(function, range, owner, this->variableRange);
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

const RPNFunction *&Function::getValue() {
	return this->function;
}


Value *Function::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot add value of type " + other->getStringType() + " to a function");
}

Value *Function::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot substract value of type " + other->getStringType() + " from a function");
}

Value *Function::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot multiply a function by a value of type " + other->getStringType());
}

Value *Function::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot divide a function by a value of type " + other->getStringType());
}

Value *Function::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot mod a function by a value of type " + other->getStringType());
}

Value *Function::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot exponentiate a function by a value of type " + other->getStringType());
}

Value *Function::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare a function to a value of type " + other->getStringType());
}

Value *Function::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare a function to a value of type " + other->getStringType());
}

Value *Function::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare a function to a value of type " + other->getStringType());
}

Value *Function::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare a function to a value of type " + other->getStringType());
}

Value *Function::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != FUNCTION)
		return new Bool(false, other->getRange(), Value::INTERPRETER);

	return new Bool(function != static_cast<const Function *>(other)->getValue(), other->getRange(), Value::INTERPRETER);
}

Value *Function::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != FUNCTION)
		return new Bool(false, other->getRange(), Value::INTERPRETER);

	return new Bool(function == static_cast<const Function *>(other)->getValue(), other->getRange(), Value::INTERPRETER);
}

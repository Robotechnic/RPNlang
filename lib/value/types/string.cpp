#include "value/types/string.hpp"

String::String(std::string_view value, TextRange range, ValueOwner owner, const TextRange variableRange) :
	Value(STRING, range, owner, variableRange),
	value(value) {}

std::unique_ptr<String> String::emptyString = std::make_unique<String>("", TextRange(), Value::EMPTY_VALUE);


inline std::string String::getStringValue() const {
	return value;
}

Value *String::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
		case ANY:
			return new String(this->value, this->range, owner);
		case BOOL:
			return new Bool(this->value.size() > 0, this->range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *String::copy(ValueOwner owner) const {
	return new String(this->value, this->range, owner, this->variableRange);
}

Value *String::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return new String(
		value + std::string(other->getStringValue()),
		TextRange::merge(range, other->getRange()),
		Value::INTERPRETER
	);
}

Value *String::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot substract value of type " + other->getStringType() + " from value of type string");
}

Value *String::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {	
	std::string result = "";
	for (int i = 0; i < static_cast<Int const*>(other)->getValue(); i++) {
		result += value;
	}

	return new String(
		result, 
		TextRange::merge(range, other->getRange()), 
		Value::INTERPRETER
	);
}

Value *String::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot divide value of type string by value of type " + other->getStringType());
}

Value *String::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot mod value of type string by value of type " + other->getStringType());
}

Value *String::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot pow value of type string by value of type " + other->getStringType());
}

Value *String::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " with value of type string");
}

Value *String::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " with value of type string");
}

Value *String::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " with value of type string");
}

Value *String::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare value of type " + other->getStringType() + " with value of type string");
}

Value *String::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRING) 
		return new Bool(
			true, 
			TextRange::merge(range, other->getRange()), 
			Value::INTERPRETER
		);
	
	return new Bool(
		value != other->getStringValue(), 
		TextRange::merge(range, other->getRange()), 
		Value::INTERPRETER
	);
}

Value *String::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRING) 
		return new Bool(
			false,
			TextRange::merge(range, other->getRange()), 
			Value::INTERPRETER
		);
	
	return new Bool(
		value == other->getStringValue(), 
		TextRange::merge(range, other->getRange()), 
		Value::INTERPRETER
	);
}

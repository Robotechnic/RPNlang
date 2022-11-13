#include "value/types/string.hpp"

String::String(std::string value, TextRange range, bool interpreterValue) :
	Value(STRING, range, interpreterValue),
	value(value) {}


bool String::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == BOOL;
}

std::string String::getStringValue() const {
	return value;
}

Value *String::to(ValueType type, bool interpreterValue) {
	switch (type) {
		case STRING:
			return new String(this->value, this->range, interpreterValue);
		case BOOL:
			return new Bool(this->value == "true", this->range, interpreterValue);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *String::copy(bool interpreterValue) const {
	return new String(this->value, this->range, interpreterValue);
}

operatorResult String::opadd(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(),
		new String(
			value + other->getStringValue(),
			TextRange::merge(this->range, other->getRange()),
			true
		)
	);
}

operatorResult String::opsub(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot substract value of type " + Value::stringType(other->getType()) + " from value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opmul(const Value *other, const Context *context) const {
	if (other->getType() != INT) 
		return std::make_tuple(
			ExpressionResult(
				"Cannot multiply value of type " + Value::stringType(other->getType()) + " with value of type string",
				other->getRange(),
				context
			),
			nullptr
		);
	
	std::string result = "";
	for (int i = 0; i < static_cast<Int const*>(other)->getValue(); i++) {
		result += value;
	}

	return std::make_tuple(
		ExpressionResult(),
		new String(
			result, 
			TextRange::merge(range, other->getRange()), 
			true
		)
	);
}

operatorResult String::opdiv(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot divide string value by value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opmod(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot modulo string value by value of type " + Value::stringType(other->getType()),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::oppow(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot power value of type " + Value::stringType(other->getType()) + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opgt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare value of type " + Value::stringType(other->getType()) + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opge(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare value of type " + Value::stringType(other->getType()) + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::oplt(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare value of type " + Value::stringType(other->getType()) + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::ople(const Value *other, const Context *context) const {
	return std::make_tuple(
		ExpressionResult(
			"Cannot compare value of type " + Value::stringType(other->getType()) + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opne(const Value *other, const Context *context) const {
	if (other->getType() != STRING) 
		return std::make_tuple(
			ExpressionResult(),
			new Bool(
				true, 
				TextRange::merge(this->range, other->getRange()), 
				true
			)
		);
	
	return std::make_tuple(
		ExpressionResult(),
		new Bool(
			value != other->getStringValue(), 
			TextRange::merge(this->range, other->getRange()), 
			true
		)
	);
}

operatorResult String::opeq(const Value *other, const Context *context) const {
	if (other->getType() != STRING) 
		return std::make_tuple(
			ExpressionResult(),
			new Bool(
				false,
				TextRange::merge(range, other->getRange()), 
				true
			)
		);
	
	return std::make_tuple(
		ExpressionResult(),
		new Bool(
			value == other->getStringValue(), 
			TextRange::merge(this->range, other->getRange()), 
			true
		)
	);
}

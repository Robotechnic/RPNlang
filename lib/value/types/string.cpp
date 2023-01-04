#include "value/types/string.hpp"

String::String(std::string_view value, TextRange range, ValueOwner owner) :
	Value(STRING, range, owner),
	value(value) {}

std::unique_ptr<String> String::emptyString = std::make_unique<String>("", TextRange(), Value::EMPTY_VALUE);

bool String::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == ANY ||
		type == BOOL;
}

inline std::string String::getStringValue() const {
	return value;
}

Value *String::to(ValueType type, ValueOwner owner) const {
	switch (type) {
		case STRING:
		case ANY:
			return new String(this->value, this->range, owner);
		case BOOL:
			return new Bool(this->value == "true", this->range, owner);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *String::copy(ValueOwner owner) const {
	return new String(this->value, this->range, owner);
}

operatorResult String::opadd(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(),
		new String(
			value + std::string(other->getStringValue()),
			TextRange::merge(this->range, other->getRange()),
			Value::INTERPRETER
		)
	);
}

operatorResult String::opsub(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot substract value of type " + other->getStringType() + " from value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opmul(const Value *other, const ContextPtr &context) const {
	if (other->getType() != INT) 
		return std::make_pair(
			ExpressionResult(
				"Cannot multiply value of type " + other->getStringType() + " with value of type string",
				other->getRange(),
				context
			),
			nullptr
		);
	
	std::string result = "";
	for (int i = 0; i < static_cast<Int const*>(other)->getValue(); i++) {
		result += value;
	}

	return std::make_pair(
		ExpressionResult(),
		new String(
			result, 
			TextRange::merge(range, other->getRange()), 
			Value::INTERPRETER
		)
	);
}

operatorResult String::opdiv(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot divide string value by value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opmod(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot modulo string value by value of type " + other->getStringType(),
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::oppow(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot power value of type " + other->getStringType() + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opgt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare value of type " + other->getStringType() + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opge(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare value of type " + other->getStringType() + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::oplt(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare value of type " + other->getStringType() + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::ople(const Value *other, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"Cannot compare value of type " + other->getStringType() + " with value of type string",
			other->getRange(),
			context
		),
		nullptr
	);
}

operatorResult String::opne(const Value *other, const ContextPtr &context) const {
	if (other->getType() != STRING) 
		return std::make_pair(
			ExpressionResult(),
			new Bool(
				true, 
				TextRange::merge(this->range, other->getRange()), 
				Value::INTERPRETER
			)
		);
	
	return std::make_pair(
		ExpressionResult(),
		new Bool(
			value != other->getStringValue(), 
			TextRange::merge(this->range, other->getRange()), 
			Value::INTERPRETER
		)
	);
}

operatorResult String::opeq(const Value *other, const ContextPtr &context) const {
	if (other->getType() != STRING) 
		return std::make_pair(
			ExpressionResult(),
			new Bool(
				false,
				TextRange::merge(range, other->getRange()), 
				Value::INTERPRETER
			)
		);
	
	return std::make_pair(
		ExpressionResult(),
		new Bool(
			value == other->getStringValue(), 
			TextRange::merge(this->range, other->getRange()), 
			Value::INTERPRETER
		)
	);
}

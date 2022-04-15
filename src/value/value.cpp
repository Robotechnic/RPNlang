#include "value/value.hpp"

Value::Value() : value(""), type(VARIABLE) {}

Value::Value(const Value &other) : valueRange(other.valueRange), value(other.value), type(other.type) {}

Value::Value(std::string value, ValueType type, int line, int column) : 
	value(value),
	type(type)
{
	if (this->value[0] == '.') {
		this->value = "0" + this->value;
	}
	this->valueRange = TextRange(line, column, value.length());
};
Value::Value(std::string value, TokenType type, int line, int column) : 
	value(value)
{
	this->valueRange = TextRange(line, column, value.length());
	switch(type) {
		case TOKEN_TYPE_INT:
			this->type = INT;
			break;
		case TOKEN_TYPE_FLOAT:
			this->type = FLOAT;
			break;
		case TOKEN_TYPE_STRING:
			this->type = STRING;
			break;
		case TOKEN_TYPE_LITERAL:
			this->type = VARIABLE;
			break;
		default:
			throw std::runtime_error("Value::Value : Invalid token type" + Token::stringType(type));
	}
	if (this->value[0] == '.') {
		this->value = "0" + this->value;
	}
}

std::string Value::getValue() const {
	return this->value;
}

ValueType Value::getType() const {
	return this->type;
}

TextRange Value::getRange() const {
	return this->valueRange;
}

std::string Value::stringType(ValueType type) {
	switch(type) {
		case INT:
			return "INT";
		case FLOAT:
			return "FLOAT";
		case STRING:
			return "STRING";
		case VARIABLE:
			return "VARIABLE";
		default:
			return "UNKNOWN";
	}
}

void Value::concatValueRange(const Value &other) {
	TextRange otherRange = other.getRange();
	if (otherRange.columnStart < this->valueRange.columnStart) {
		this->valueRange.columnStart = otherRange.columnStart;
	}

	if (otherRange.columnEnd > this->valueRange.columnEnd) {
		this->valueRange.columnEnd = otherRange.columnEnd;
	}
}

ExpressionResult Value::setVariable(std::map<std::string, Value> &variables) {
	if (this->type != VARIABLE) return ExpressionResult();

	if (variables.find(this->value) == variables.end()) {
		return ExpressionResult(
			"Variable " + this->value + " not found",
			this->valueRange
		);
	}
	this->type = variables[this->value].getType();
	this->value = variables[this->value].getValue();
	return ExpressionResult();
}

ExpressionResult Value::add(const Value &other) {
	this->concatValueRange(other);
	if (this->type == STRING || other.getType() == STRING) {
		if (this->type == STRING && other.getType() == STRING) {
			this->value += other.getValue();
			return ExpressionResult();
		}
		return ExpressionResult(
			"Cannot add " + Value::stringType(this->type) + " and " + Value::stringType(other.getType()),
			this->valueRange
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) + std::stof(other.getValue())
		);
		return ExpressionResult();
	}

	this->value = std::to_string(
		std::stoi(this->value) + std::stoi(other.getValue())
	);
	return ExpressionResult();
}

ExpressionResult Value::substract(const Value &other) {
	this->concatValueRange(other);
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Cannot substract " + Value::stringType(this->type) + " and " + Value::stringType(other.getType()),
			this->valueRange
		);
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) - std::stof(other.getValue())
		);
		return ExpressionResult();
	}

	this->value = std::to_string(
		std::stoi(this->value) - std::stoi(other.getValue())
	);
	return ExpressionResult();
}

ExpressionResult Value::multiply(const Value &other) {
	this->concatValueRange(other);
	if (other.getType() == STRING) {
		return ExpressionResult(
			"Cannot multiply " + Value::stringType(this->type) + " and " + Value::stringType(other.getType()),
			this->valueRange
		);
	}
	if (this->type == STRING && other.getType() == INT) {
		std::string result = "";
		for (int i = 0; i < std::stoi(other.getValue()); i++) {
			result += this->value;
		}
		this->value = result;
		return ExpressionResult();
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) * std::stof(other.getValue())
		);
		return ExpressionResult();
	}

	this->value = std::to_string(
		std::stoi(this->value) * std::stoi(other.getValue())
	);
	return ExpressionResult();
}

ExpressionResult Value::divide(const Value &other) {
	this->concatValueRange(other);
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Cannot divide " + Value::stringType(this->type) + " and " + Value::stringType(other.getType()),
			this->valueRange
		);
	}

	if (stof(other.getValue()) == 0.0) {
		return ExpressionResult(
			"Cannot divide by 0",
			other.valueRange
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) / std::stof(other.getValue())
		);
		return ExpressionResult();
	}

	this->value = std::to_string(
		std::stoi(this->value) / std::stoi(other.getValue())
	);
	return ExpressionResult();
}
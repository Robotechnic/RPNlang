#include "interpreter/value.hpp"

Value::Value() : value(""), type(VARIABLE) {}
Value::Value(std::string value, ValueType type, int line, int column) : 
	value(value),
	type(type),
	line(line),
	column(column)
{
	if (this->value[0] == '.') {
		this->value = "0" + this->value;
	}
};
Value::Value(std::string value, TokenType type, int line, int column) : 
	value(value),
	line(line),
	column(column) 
{
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

ExpressionResult Value::setVariable(std::map<std::string, Value> &variables) {
	if (this->type != VARIABLE) return ExpressionResult(*this);

	if (variables.find(this->value) == variables.end()) {
		return ExpressionResult(
			"Variable " + this->value + " not found",
			this->line,
			this->column
		);
	}
	this->type = variables[this->value].getType();
	this->value = variables[this->value].getValue();
	return ExpressionResult(*this);
}

ExpressionResult Value::stringTest(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		if (this->type == STRING && other.getType() == STRING) {
			this->value += other.getValue();
			return ExpressionResult(*this);
		}
		return ExpressionResult(
			"Cannot add " + Value::stringType(this->type) + " and " + Value::stringType(other.getType()),
			this->line,
			this->column
		);
	}
	return ExpressionResult(*this);
}

ExpressionResult Value::add(const Value &other) {
	

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) + std::stof(other.getValue())
		);
		return true;
	}

	this->value = std::to_string(
		std::stoi(this->value) + std::stoi(other.getValue())
	);
	return true;
}

ExpressionResult Value::substract(const Value &other) {
	if (this->type == FLOAT or other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) - std::stof(other.getValue())
		);
		return true;
	}

	this->value = std::to_string(
		std::stoi(this->value) - std::stoi(other.getValue())
	);
	return true;
}

ExpressionResult Value::multiply(const Value &other) {
	if (this->type == FLOAT or other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) * std::stof(other.getValue())
		);
		return true;
	}

	this->value = std::to_string(
		std::stoi(this->value) * std::stoi(other.getValue())
	);
	return true;
}

ExpressionResult Value::divide(const Value &other) {
	if (std::stof(other.getValue()) == 0.0) {
		return false;
	}

	if (this->type == FLOAT or other.getType() == FLOAT) {
		this->value = std::to_string(
			std::stof(this->value) / std::stof(other.getValue())
		);
		return true;
	}

	this->value = std::to_string(
		std::stoi(this->value) / std::stoi(other.getValue())
	);
	return true;
}
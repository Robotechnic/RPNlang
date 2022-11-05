#include "value/value.hpp"

Value::Value(ValueType type, const TextRange range, bool interpreterValue) :
	interpreterValue(interpreterValue),
	range(range),
	type(type) {}

Value::~Value() {}

ValueType Value::getType() const {
	return this->type;
}

TextRange Value::getRange() const {
	return this->range;
}

Value *Value::to(ValueType type) {
	throw std::runtime_error("Cannot convert " + this->stringType(this->getType()) + " to " + this->stringType(type));
}

/**
 * @brief return the string representation of the ValueType
 * 
 * @param type the type to convert to a string
 * @return std::string the string representation of the type
 */
std::string Value::stringType(const ValueType type) {
	switch(type) {
		case INT:
			return "int";
		case FLOAT:
			return "float";
		case STRING:
			return "string";
		case BOOL:
			return "bool";
		case FUNCTION:
			return "function";
		case NONE:
			return "none";
		case VARIABLE:
			return "variable";
		case PATH:
			return "path";
		default:
			return "<Invalid value type>";
	}
}

/**
 * @brief convert string representation of a value type to a ValueType
 * 
 * @param type the string representation of the type
 * @return ValueType the type
 */
ValueType Value::valueType(const std::string type) {
	if (type == "int")
		return INT;
	if (type == "float")
		return FLOAT;
	if (type == "string")
		return STRING;
	if (type == "function")
		return FUNCTION;
	if (type == "bool")
		return BOOL;
	if (type == "none")
		return NONE;
	
	throw std::runtime_error("This type doesn't exist");
}

void Value::concatValueRange(const Value *other) {
	this->range.merge(other->getRange());
}

void Value::concatValueRange(const Token *other) {
	this->range.merge(other->getRange());
}

/**
 * @brief apply a math operation to the value
 * 
 * @param other the other value to apply the operation to
 * @param operatorToken the operator token to apply
 * @param variables variables to use in the operation
 * @return ExpressionResult if the operation was successful
 */
operatorResult Value::applyOperator(const Value *other, const Token *operatorToken, const Context *context) {
	std::string op = operatorToken->getStringValue();

	this->concatValueRange(other);
	this->concatValueRange(operatorToken);

	if (op == "/")
		return this->opdiv(other, context);
	if (op == "%")
		return this->opmod(other, context);
	if (op == "+")
		return this->opadd(other, context);
	if (op == "-")
		return this->opsub(other, context);
	if (op == "*")
		return this->opmul(other, context);
	if (op == "^")
		return this->oppow(other, context);
	if (op == "==")
		return this->opeq(other, context);
	if (op == "!=")
		return this->opne(other, context);
	if (op == "<")
		return this->oplt(other, context);
	if (op == "<=")
		return this->ople(other, context);
	if (op == ">")
		return this->opgt(other, context);
	if (op == ">=")
		return this->opge(other, context);

	return std::make_tuple(
		ExpressionResult(
			"Invalid operator " + op,
			this->getRange(),
			context
		),
		nullptr
	);
}

std::ostream &operator<<(std::ostream &os, const Value *value) {
	os << "( " << Value::stringType(value->getType()) << "," << value->getStringValue() << " )";
	return os;
}

std::string std::to_string(const Value *value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

/**
 * @brief delete tthe value in parameters if it is an INterpreter generated value
 * 
 * @param val the value to delete
 */
void Value::deleteValue(Value **val) {
	if (!(*val)->interpreterValue) return;
	delete (*val);
	(*val) = nullptr;
}
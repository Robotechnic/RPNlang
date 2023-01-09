#include "value/value.hpp"

Value::Value(ValueType type, const TextRange range, ValueOwner owner) :
	range(range),
	variableRange(range),
	type(type),
	owner(owner) {
		if (type == ANY) {
			throw std::runtime_error("ANY type is not allowed in value type");
		}
	}


ValueType Value::getType() const {
	return this->type;
}

std::string Value::getStringType() const {
	return std::string(Value::stringType(this->type));
}

TextRange Value::getRange() const {
	if (this->variableRange == this->range) {
		return this->range;
	}
	return this->variableRange;
}

TextRange Value::getVariableRange() const {
	return this->variableRange;
}

void Value::setVariableRange(const TextRange &range) {
	this->variableRange = range;
}

/**
 * @brief return the string representation of the ValueType
 * 
 * @param type the type to convert to a string
 * @return std::string the string representation of the type
 */
std::string Value::stringType(const ValueType &type) {
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
		case LIST:
			return "list";
		case ANY:
			return "any";
		
		default:
			throw std::runtime_error("This type doesn't exist " + std::to_string(type));
	}
}

/**
 * @brief convert string representation of a value type to a ValueType
 * 
 * @param type the string representation of the type
 * @return ValueType the type
 */
ValueType Value::valueType(std::string_view type) {
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
	if (type == "list")
		return LIST;
	
	throw std::runtime_error("This string type doesn't exist");
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
operatorResult Value::applyOperator(const Value *other, const Token *operatorToken, const ContextPtr &context) {
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

	return std::make_pair(
		ExpressionResult(
			"Invalid operator " + op,
			this->getRange(),
			context
		),
		nullptr
	);
}

Value::ValueOwner Value::getOwner() const {
	return this->owner;
}

void Value::setOwner(ValueOwner owner, bool overwrite) {
	if (overwrite || owner > this->owner) {
		this->owner = owner;
	}
}

/**
 * @brief delete tthe value in parameters if it is an INterpreter generated value
 * 
 * @param val the value to delete
 */
void Value::deleteValue(Value **val, ValueOwner deleter) {
	if ((*val) == nullptr || (*val)->owner != deleter) 
		return;
	delete (*val);
	(*val) = nullptr;
}

std::ostream &operator<<(std::ostream &os, const Value *value) {
	os << "( " << value->getStringType() << "," << value->getStringValue() << " )";
	return os;
}

std::string std::to_string(const Value *value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}
#include "value/value.hpp"

Value::Value(ValueType type, const TextRange range, ValueOwner owner, const TextRange variableRange) :
	range(range),
	type(type),
	owner(owner) {
		if (type == ANY) {
			throw std::runtime_error("ANY type is not allowed in value type");
		}
		if (variableRange == TextRange()) {
			this->variableRange = range;
		} else {
			this->variableRange = variableRange;
		}
	}


ValueType Value::getType() const {
	return this->type;
}

void Value::setType(ValueType type) {
	this->type = type;
}

bool Value::isCastableTo(ValueType from, ValueType to) {
	if (from == to || to == ANY || to == BOOL) {
		return true;
	}
	switch (from) {
		case INT:
		case FLOAT:
		case BOOL:
			return to == INT || to == FLOAT || to == BOOL || to == STRING;
		case STRING:
			return to == STRING || to == BOOL;
		case LIST:
			return to == LIST || to == STRING || to == BOOL;
		default:
			return false;
	}
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

void Value::setVariableRange(const TextRange &&range) {
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
		case STRUCT:
			return "struct";
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
Value* Value::applyOperator(const Value *other, const Token *operatorToken, const ContextPtr &context) {
	OperatorToken::OperatorTypes op = static_cast<const OperatorToken*>(operatorToken)->getOperatorType();

	TextRange range = TextRange::merge(this->getRange(), other->getRange());
	range.merge(operatorToken->getRange());

	switch (op) {
		case OperatorToken::OP_ADD:
			return this->opadd(other, range, context);
			break;
		case OperatorToken::OP_SUB:
			return this->opsub(other, range, context);
			break;
		case OperatorToken::OP_MUL:
			return this->opmul(other, range, context);
			break;
		case OperatorToken::OP_DIV:
			return this->opdiv(other, range, context);
			break;
		case OperatorToken::OP_MOD:
			return this->opmod(other, range, context);
			break;
		case OperatorToken::OP_POW:
			return this->oppow(other, range, context);
			break;
		case OperatorToken::OP_EQ:
			return this->opeq(other, range, context);
			break;
		case OperatorToken::OP_NE:
			return this->opne(other, range, context);
			break;
		case OperatorToken::OP_GT:
			return this->opgt(other, range, context);
			break;
		case OperatorToken::OP_GE:
			return this->opge(other, range, context);
			break;
		case OperatorToken::OP_LT:
			return this->oplt(other, range, context);
			break;
		case OperatorToken::OP_LE:
			return this->ople(other, range, context);
			break;
	}

	throw std::runtime_error("This operator doesn't exist");
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

std::ostream &operator<<(std::ostream &os, const RPNValueType &type) {
	if (type.index() == 0) {
		os << std::get<std::string>(type);
	} else {
		os << Value::stringType(std::get<ValueType>(type));
	}
	return os;
}

std::string std::to_string(const Value *value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}
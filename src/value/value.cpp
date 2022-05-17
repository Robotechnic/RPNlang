#include "value/value.hpp"


Value::Value() : value(0), type(NONE) {}

Value::Value(const Value &other, const Context &context) : value(other.value), valueRange(other.valueRange), type(other.type) {}

Value::Value(std::string value, ValueType type, int line, int column) : 
	valueRange(line, column, value.length()),
	type(type)
{
	if (type == FLOAT && value[0] == '.') {
		value = "0" + value;
	}

	switch (type) {
		case INT:
			this->value = (int)std::stoi(value);
			break;
		case FLOAT:
			this->value = (float)std::stof(value);
			break;
		case BOOL:
			this->value = (bool)(value == "true");
			break;
		case STRING:
			this->value = value;
			break;
		default:
			throw std::runtime_error("Invalid value type");
	}
};

Value::Value(std::string value, TokenType type, int line, int column) {
	this->valueRange = TextRange(line, column, value.length());
	if (value[0] == '.') {
		value = "0" + value;
	}
	
	switch (type) {
		case TOKEN_TYPE_INT:
			this->value = (int)std::stoi(value);
			this->type = INT;
			break;
		case TOKEN_TYPE_FLOAT:
			this->value = (float)std::stof(value);
			this->type = FLOAT;
			break;
		case TOKEN_TYPE_BOOL:
			this->value = (bool)(value == "true");
			this->type = BOOL;
			break;
		case TOKEN_TYPE_STRING:
			this->value = value;
			this->type = STRING;
			break;
		case TOKEN_TYPE_LITERAL:
			this->value = (std::string)value;
			this->type = VARIABLE;
			break;
		default:
			throw std::runtime_error("Invalid value type");
	}
}

Value::Value(std::string value, int line, int column) :
	value(value),
	valueRange(line, column, value.size()),
	type(STRING) {}

Value::Value(std::string value, TextRange range) :
	value(value),
	valueRange(range),
	type(STRING) {}

Value::Value(int value, int line, int column) : 
	value(value),
	valueRange(line, column, std::to_string(value).length()),
	type(INT) {}

Value::Value(float value, int line, int column) : 
	value(value),
	valueRange(line, column, std::to_string(value).length()),
	type(FLOAT) {}
	
Value::Value(bool value, int line, int column) : 
	value(value),
	valueRange(line, column, std::to_string(value).length()),
	type(BOOL) {}

Value::Value(RPNFunction * function, int line, int column) :
	value(function),
	valueRange(function->getRange()),
	type(FUNCTION) {}

Value::Value(int value, TextRange range) : 
	value(value),
	valueRange(range),
	type(INT) {}

Value::Value(float value, TextRange range) : 
	value(value),
	valueRange(range),
	type(FLOAT) {}
	
Value::Value(bool value, TextRange range) : 
	value(value),
	valueRange(range),
	type(BOOL) {}

Value::Value(RPNFunction * function, TextRange range) :
	value(function),
	valueRange(function->getRange()),
	type(FUNCTION) {}

/**
 * @brief return the value if it is a float or try to convert it to a float
 * 
 * @return float the value as a float
 */
float Value::getFloatValue() const {
	switch (this->type) {
		case INT:
			return (float)std::get<int>(this->value);
		case FLOAT:
			return std::get<float>(this->value);
		case BOOL:
			return (float)std::get<bool>(this->value);
		case STRING:
			try {
				return std::stod(std::get<std::string>(this->value));
			} catch (std::invalid_argument &e) {
				throw std::runtime_error("Value::getFloatValue : Invalid string value");
			}
		default:
			throw std::runtime_error("Value::getFloatValue : This value is not convertible to float");
	}
}

/**
 * @brief return the value if it is an int or try to convert it to an int
 * 
 * @return int the value as an int
 */
int Value::getIntValue() const {
	switch (this->type) {
		case INT:
			return std::get<int>(this->value);
		case FLOAT:
			return (int)std::get<float>(this->value);
		case BOOL:
			return (int)std::get<bool>(this->value);
		case STRING:
			try {
				return std::stoi(std::get<std::string>(this->value));
			} catch (std::invalid_argument &e) {
				throw std::runtime_error("Value::getIntValue : Invalid string value");
			}
		default:
			throw std::runtime_error("Value::getIntValue : This value is not convertible to integer");
	}
}

/**
 * @brief return the value if it is a bool or try to convert it to a bool
 * 
 * @return bool the value as a bool
 */
bool Value::getBoolValue() const {
	switch (this->type) {
		case INT:
			return std::get<int>(this->value) != 0;
		case FLOAT:
			return std::get<float>(this->value) != 0;
		case BOOL:
			return std::get<bool>(this->value);
		case STRING:
			return std::get<std::string>(this->value) != "";
		default:
			throw std::runtime_error("This value is not convertible to boolean");
	}
}

/**
 * @brief return the value if it is a string or convert it to a string
 * 
 * @return std::string the value as a string
 */
std::string Value::getStringValue() const {
	switch (this->type) {
		case INT:
			return std::to_string(std::get<int>(this->value));
		case FLOAT:
			return std::to_string(std::get<float>(this->value));
		case BOOL:
			return std::get<bool>(this->value) ? "true" : "false";
		case STRING:
		case VARIABLE:
			return std::get<std::string>(this->value);
		case NONE:
			return "";
		case FUNCTION:
			const RPNFunction* function = std::get<RPNFunction *>(this->value);
			if (function->getName() == "") {
				return "<anonymous function>";
			}
			return "<function " + function->getName() + ">";
	}
	throw std::runtime_error("This value is not convertible to string");
}

/**
 * @brief return the value if it is a function
 * 
 * @return RPNFunction* the value as a function
 */
const RPNFunction* Value::getFunctionValue() const {
	if (this->type == FUNCTION) {
		return std::get<RPNFunction*>(this->value);
	}

	throw std::runtime_error("This value is not a function");
}

/**
 * @brief check if the current value can be converted to a given type
 * 
 * @param type the type to convert to
 * @return true if the value can be converted to the given type
 */
bool Value::isCastableTo(ValueType type) const {
	switch (type) {
		case INT:
		case FLOAT:
		case BOOL:
			return this->isNumber();
		case STRING:
			return true;
		default:
			return false;
	}
}
/**
 * @brief check if the current value is a number
 * 
 * @return true if the value is a number
 */
bool Value::isNumber() const {
	if (this->type == INT || this->type == FLOAT || this->type == BOOL) {
		return true;
	}

	if (this->type == STRING && std::regex_match(std::get<std::string>(this->value), std::regex("^[0-9](.[0-9]+)?$"))) {
		return true;
	}

	return false;
}

ValueType Value::getType() const {
	return this->type;
}

TextRange Value::getRange() const {
	return this->valueRange;
}

ValueStorage Value::getValue() const {
	return this->value;
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
		case VARIABLE:
			return "variable";
		case FUNCTION:
			return "function";
		case NONE:
			return "none";
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
	if (type == "variable")
		return VARIABLE;
	if (type == "function")
		return FUNCTION;
	if (type == "none")
		return NONE;
	
	throw std::runtime_error("This type doesn't exist");
}

void Value::concatValueRange(const Value &other) {
	this->valueRange.merge(other.getRange());
}

void Value::concatValueRange(const Token &other) {
	this->valueRange.merge(other.getRange());
}

void Value::setValue(std::string value) {
	this->value = value;
	this->type = STRING;
}

void Value::setValue(float value) {
	this->value = value;
	this->type = FLOAT;
}

void Value::setValue(int value) {
	this->value = value;
	this->type = INT;
}

void Value::setValue(bool value) {
	this->value = value;
	this->type = BOOL;
}

void Value::setValue(RPNFunction * function) {
	this->value = function;
	this->type = FUNCTION;
}

/**
 * @brief check if the value is a valid variable name and if it is, set it's value and type
 * 
 * @param context the context where the symbol table is defined
 * @return ExpressionResult if the variable is valid
 */
ExpressionResult Value::getVariableValue(const Context &context) {
	if (this->type != VARIABLE) return ExpressionResult();

	Value temp;
	ExpressionResult result = context.getValue(*this, temp);
	if (result.error()) return result;

	this->value = temp.getValue();
	this->type = temp.getType();
	return ExpressionResult();
}


/**
 * @brief apply a math operation to the value
 * 
 * @param other the other value to apply the operation to
 * @param operatorToken the operator token to apply
 * @param variables variables to use in the operation
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::applyOperator(const Value &other, const Token &operatorToken, const Context &context) {
	std::string op = operatorToken.getValue();

	if (this->type == NONE) {
		return ExpressionResult(
			"Invalid operator " + op + " on NONE value",
			this->getRange(),
			context
		);
	}

	if (other.getType() == NONE) {
		return ExpressionResult(
			"Invalid operator " + op + " on NONE value",
			other.getRange(),
			context
		);
	}

	if (this->type == VARIABLE) {
		ExpressionResult result = this->getVariableValue(context);
		if (result.error()) return result;
	}

	Value otherValue = other;

	if (other.getType() == VARIABLE) {
		ExpressionResult result = otherValue.getVariableValue(context);
		if (result.error()) return result;
	}

	this->concatValueRange(other);
	this->concatValueRange(operatorToken);

	if (op == "/")
		return this->opdiv(otherValue, context);
	if (op == "%")
		return this->opmod(otherValue, context);
	if (op == "+")
		return this->opadd(otherValue, context);
	if (op == "-")
		return this->opsub(otherValue, context);
	if (op == "*")
		return this->opmul(otherValue, context);
	if (op == "^")
		return this->oppow(otherValue, context);
	if (op == "==")
		return this->opeq(otherValue, context);
	if (op == "!=")
		return this->opne(otherValue, context);
	if (op == "<")
		return this->oplt(otherValue, context);
	if (op == "<=")
		return this->ople(otherValue, context);
	if (op == ">")
		return this->opgt(otherValue, context);
	if (op == ">=")
		return this->opge(otherValue, context);

	return ExpressionResult(
		"Invalid operator " + op,
		this->getRange(),
		context
	);
}

/**
 * @brief add another value to this value
 * 
 * @param other the other value to add
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opadd(const Value &other, const Context &context) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() + other.getStringValue()
		);
	} else if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator + between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	} else if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() + other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() + other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief subtract another value from this value
 * 
 * @param other the other value to subtract
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opsub(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator - between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() - other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() - other.getIntValue()
		);
	}
	return ExpressionResult();
}

/**
 * @brief multiply this value by another value
 * 
 * @param other the other value to multiply by
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opmul(const Value &other, const Context &context) {
	if (this->type == STRING && other.getType() == INT) {
		std::string result = "";
		for (int i = 0; i < other.getIntValue(); i++) {
			result += std::get<std::string>(this->value);
		}
		this->setValue(result);
		return ExpressionResult();
	} else if (other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator * between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() * other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() * other.getIntValue()
		);
	}
	return ExpressionResult();
}

/**
 * @brief divide this value by another value
 * 
 * @param other the other value to divide by
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opdiv(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator / between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (other.getFloatValue() == 0.0) {
		return ExpressionResult(
			"Division by zero",
			other.getRange(),
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() / other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() / other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief modulo this value by another value
 * 
 * @param other the other value to modulo by
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opmod(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator % between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (other.getFloatValue() == 0.0) {
		return ExpressionResult(
			"Division by zero",
			other.getRange(),
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			std::fmod(this->getFloatValue(), other.getFloatValue())
		);
	} else {
		this->setValue(
			this->getIntValue() % other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief power this value by another value
 * 
 * @param other the other value to power by
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::oppow(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator ^ between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			std::pow(this->getFloatValue(), other.getFloatValue())
		);
	} else {
		this->setValue(
			(int)std::pow(this->getIntValue(), other.getIntValue())
		);
	}

	return ExpressionResult();
}

/**
 * @brief check if this value is greater than other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opgt(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator > between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() > other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() > other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief check if this value is greater than or equal to other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opge(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator >= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() >= other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() >= other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief check if this value is less than other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::oplt(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator < between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() < other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() < other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief check if this value is less than or equal to other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::ople(const Value &other, const Context &context) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator <= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange,
			context
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() <= other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() <= other.getIntValue()
		);
	}

	return ExpressionResult();
}

/**
 * @brief check if this value is not equal to other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opne(const Value &other, const Context &context) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() != other.getStringValue()
		);
	} else if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() != other.getFloatValue()
		);
	} else if (this->isNumber() && other.isNumber()) {
		this->setValue(
			this->getIntValue() != other.getIntValue()
		);
	} else {
		this->setValue(true);
	}
	return ExpressionResult();
}

/**
 * @brief check if this value is equal to other value
 * 
 * @param other the other value to compare to
 * @return ExpressionResult if the operation was successful
 */
ExpressionResult Value::opeq(const Value &other, const Context &context) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() == other.getStringValue()
		);
	} else if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() == other.getFloatValue()
		);
	} else if (this->isNumber() && other.isNumber()) {
		this->setValue(
			this->getIntValue() == other.getIntValue()
		);
	} else {
		this->setValue(false);
	}

	return ExpressionResult();
}

Value Value::operator+(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator + between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return Value(
			this->getFloatValue() + other.getFloatValue(),
			this->getRange().merge(other.getRange())
		);
	}
	return Value(
		this->getIntValue() + other.getIntValue(),
		this->getRange().merge(other.getRange())
	);
}

Value Value::operator-(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator - between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return Value(
			this->getFloatValue() - other.getFloatValue(),
			this->getRange().merge(other.getRange())
		);
	}
	return Value(
		this->getIntValue() - other.getIntValue(),
		this->getRange().merge(other.getRange())
	);
}

Value Value::operator-() {
	if (!this->isNumber()) {
		throw std::runtime_error("Invalid operator - on " + this->stringType(this->type));
	}
	
	if (this->type == FLOAT) {
		return Value(
			-this->getFloatValue(),
			this->getRange()
		);
	}

	return Value(
		-this->getIntValue(),
		this->getRange()
	);
}

Value Value::operator*(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator * between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return Value(
			this->getFloatValue() * other.getFloatValue(),
			this->getRange().merge(other.getRange())
		);
	}
	return Value(
		this->getIntValue() * other.getIntValue(),
		this->getRange().merge(other.getRange())
	);
}

Value Value::operator/(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator / between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return Value(
			this->getFloatValue() / other.getFloatValue(),
			this->getRange().merge(other.getRange())
		);
	}
	return Value(
		this->getIntValue() / other.getIntValue(),
		this->getRange().merge(other.getRange())
	);
}

bool Value::operator>(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator > between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return this->getFloatValue() > other.getFloatValue();
	}
	return this->getIntValue() > other.getIntValue();
}

bool Value::operator>=(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator >= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return this->getFloatValue() >= other.getFloatValue();
	}
	return this->getIntValue() >= other.getIntValue();
}

bool Value::operator<(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator < between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return this->getFloatValue() < other.getFloatValue();
	}
	return this->getIntValue() < other.getIntValue();
}

bool Value::operator<=(const Value &other) {
	if (!this->isNumber() || !other.isNumber()) {
		throw std::runtime_error("Invalid operator <= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()));
	}
	if (this->type == FLOAT || other.getType() == FLOAT) {
		return this->getFloatValue() <= other.getFloatValue();
	}
	return this->getIntValue() <= other.getIntValue();
}

bool Value::operator!=(const Value &other) {
	if (this->getType() != other.getType()) {
		return true;
	}

	return this->value != other.value;
}

bool Value::operator==(const Value &other) {
	if (this->getType() != other.getType()) {
		return false;
	}

	return this->value == other.value;
}

Value &operator+=(Value &lhs, const Value &rhs) {
	if (!lhs.isNumber() || !rhs.isNumber()) {
		throw std::runtime_error("Invalid operator += between " + lhs.stringType(lhs.getType()) + " and " + lhs.stringType(rhs.getType()));
	}
	if (lhs.getType() == FLOAT || rhs.getType() == FLOAT) {
		lhs.setValue(lhs.getFloatValue() + rhs.getFloatValue());
		lhs.concatValueRange(rhs);
		return lhs;
	}

	lhs.setValue(lhs.getIntValue() + rhs.getIntValue());
	lhs.concatValueRange(rhs);
	return lhs;
}

Value &operator-=(Value &lhs, const Value &rhs) {
	if (!lhs.isNumber() || !rhs.isNumber()) {
		throw std::runtime_error("Invalid operator -= between " + lhs.stringType(lhs.getType()) + " and " + lhs.stringType(rhs.getType()));
	}
	if (lhs.getType() == FLOAT || rhs.getType() == FLOAT) {
		lhs.setValue(lhs.getFloatValue() - rhs.getFloatValue());
		lhs.concatValueRange(rhs);
		return lhs;
	}

	lhs.setValue(lhs.getIntValue() - rhs.getIntValue());
	lhs.concatValueRange(rhs);
	return lhs;
}
Value &operator*=(Value &lhs, const Value &rhs) {
	if (!lhs.isNumber() || !rhs.isNumber()) {
		throw std::runtime_error("Invalid operator *= between " + lhs.stringType(lhs.getType()) + " and " + lhs.stringType(rhs.getType()));
	}
	if (lhs.getType() == FLOAT || rhs.getType() == FLOAT) {
		lhs.setValue(lhs.getFloatValue() * rhs.getFloatValue());
		lhs.concatValueRange(rhs);
		return lhs;
	}

	lhs.setValue(lhs.getIntValue() * rhs.getIntValue());
	lhs.concatValueRange(rhs);
	return lhs;
}

Value &operator/=(Value &lhs, const Value &rhs) {
	if (!lhs.isNumber() || !rhs.isNumber()) {
		throw std::runtime_error("Invalid operator /= between " + lhs.stringType(lhs.getType()) + " and " + lhs.stringType(rhs.getType()));
	}
	if (lhs.getType() == FLOAT || rhs.getType() == FLOAT) {
		lhs.setValue(lhs.getFloatValue() / rhs.getFloatValue());
		lhs.concatValueRange(rhs);
		return lhs;
	}

	lhs.setValue(lhs.getIntValue() / rhs.getIntValue());
	lhs.concatValueRange(rhs);
	return lhs;
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
	os << "( " << value.getType() << "," << value.getStringValue() << " )";
	return os;
}

std::string std::to_string(const Value &value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}
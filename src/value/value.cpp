#include "value/value.hpp"


Value::Value() : value(0), type(NONE) {}

Value::Value(const Value &other) : value(other.value), valueRange(other.valueRange), type(other.type) {}

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
			this->value = (std::string)value;
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
			this->value = (std::string)value;
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
	return "";
}

const RPNFunction* Value::getFunctionValue() const {
	if (this->type == FUNCTION) {
		return std::get<RPNFunction*>(this->value);
	}

	throw std::runtime_error("This value is not a function");
}

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

bool Value::isNumber() const {
	if (this->type == INT || this->type == FLOAT || this->type == BOOL) {
		return true;
	}

	if (this->type == STRING && std::regex_match(std::get<std::string>(this->value), std::regex("^[0-9]+$"))) {
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

void Value::concatValueRange(const TextRange &otherRange) {
	if (otherRange.columnStart < this->valueRange.columnStart) {
		this->valueRange.columnStart = otherRange.columnStart;
	}

	if (otherRange.columnEnd > this->valueRange.columnEnd) {
		this->valueRange.columnEnd = otherRange.columnEnd;
	}
}

void Value::concatValueRange(const Value &other) {
	this->concatValueRange(other.getRange());
}

void Value::concatValueRange(const Token &other) {
	this->concatValueRange(other.getRange());
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

ExpressionResult Value::setVariable(std::map<std::string, Value> &variables) {
	if (this->type != VARIABLE) return ExpressionResult();

	std::string varName = this->getStringValue();
	if (variables.find(varName) == variables.end()) {
		return ExpressionResult(
			"Variable " + varName + " not found",
			this->valueRange
		);
	}

	this->type = variables[varName].getType();
	this->value = variables[varName].getValue();

	return ExpressionResult();
}

ExpressionResult Value::applyOperator(const Value &other, const Token &operatorToken, std::map<std::string, Value> &variables) {
	std::string op = operatorToken.getValue();

	if (this->type == NONE) {
		return ExpressionResult(
			"Invalid operator " + op + " on NONE value",
			this->getRange()
		);
	}

	if (other.getType() == NONE) {
		return ExpressionResult(
			"Invalid operator " + op + " on NONE value",
			other.getRange()
		);
	}

	if (this->type == VARIABLE) {
		ExpressionResult result = this->setVariable(variables);
		if (result.error()) return result;
	}

	Value otherValue = other;

	if (other.getType() == VARIABLE) {
		ExpressionResult result = otherValue.setVariable(variables);
		if (result.error()) return result;
	}

	this->concatValueRange(other);
	this->concatValueRange(operatorToken);

	if (op == "/")
		return this->opdiv(otherValue);
	if (op == "%")
		return this->opmod(otherValue);
	if (op == "+")
		return this->opadd(otherValue);
	if (op == "-")
		return this->opsub(otherValue);
	if (op == "*")
		return this->opmul(otherValue);
	if (op == "^")
		return this->oppow(otherValue);
	if (op == "==")
		return this->opeq(otherValue);
	if (op == "!=")
		return this->opne(otherValue);
	if (op == "<")
		return this->oplt(otherValue);
	if (op == "<=")
		return this->ople(otherValue);
	if (op == ">")
		return this->opgt(otherValue);
	if (op == ">=")
		return this->opge(otherValue);

	return ExpressionResult(
		"Invalid operator " + op,
		this->getRange()
	);
}

ExpressionResult Value::opadd(const Value &other) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() + other.getStringValue()
		);
	} else if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator + between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::opsub(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator - between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::opmul(const Value &other) {
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
			this->valueRange
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

ExpressionResult Value::opdiv(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator / between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
		);
	}

	if (other.getFloatValue() == 0.0) {
		return ExpressionResult(
			"Division by zero",
			other.getRange()
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

ExpressionResult Value::opmod(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator % between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
		);
	}

	if (other.getFloatValue() == 0.0) {
		return ExpressionResult(
			"Division by zero",
			other.getRange()
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

ExpressionResult Value::oppow(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator ^ between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::opgt(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator > between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::opge(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator >= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::oplt(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator < between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::ople(const Value &other) {
	if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator <= between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
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

ExpressionResult Value::opne(const Value &other) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() != other.getStringValue()
		);
	} else if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator != between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() != other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() != other.getIntValue()
		);
	}

	return ExpressionResult();
}

ExpressionResult Value::opeq(const Value &other) {
	if (this->type == STRING && other.getType() == STRING) {
		this->setValue(
			this->getStringValue() == other.getStringValue()
		);
	} else if (this->type == STRING || other.getType() == STRING) {
		return ExpressionResult(
			"Invalid operator == between " + this->stringType(this->type) + " and " + this->stringType(other.getType()),
			this->valueRange
		);
	}

	if (this->type == FLOAT || other.getType() == FLOAT) {
		this->setValue(
			this->getFloatValue() == other.getFloatValue()
		);
	} else {
		this->setValue(
			this->getIntValue() == other.getIntValue()
		);
	}

	return ExpressionResult();
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
#include "tokens/tokens/valuetoken.hpp"

ValueToken::ValueToken(Value *value, TokenType type) : 
	Token(value->getRange(), type),
	value(value) {
		value->setOwner(Value::VALUE_TOKEN, true);
	}

ValueToken::~ValueToken() {
	Value::deleteValue(&this->value, Value::VALUE_TOKEN);
}

Value *&ValueToken::getValue() {
	return this->value;
}

inline std::string ValueToken::getStringValue() const {
	return this->value->getStringValue();
}

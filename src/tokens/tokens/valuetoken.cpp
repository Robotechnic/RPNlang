#include "tokens/tokens/valuetoken.hpp"

ValueToken::ValueToken(Value *value) : 
	Token(value->getRange(), TOKEN_TYPE_VALUE),
	value(value) {
		value->interpreterValue = false;
	}

ValueToken::~ValueToken() {
	delete this->value;
}

Value *&ValueToken::getValue() {
	return this->value;
}

std::string ValueToken::getStringValue() const {
	return this->value->getStringValue();
}

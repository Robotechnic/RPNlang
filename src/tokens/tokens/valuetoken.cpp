#include "tokens/tokens/valuetoken.hpp"

ValueToken::ValueToken(Value *value) : Token(value->getRange(), TOKEN_TYPE_VALUE), accessed(false), value(value) {}

ValueToken::~ValueToken() {
	if (!this->accessed) delete this->value;
}

Value *ValueToken::getValue() {
	this->accessed = true;
	return this->value;
}

std::string ValueToken::getStringValue() const {
	return this->value->getStringValue();
}

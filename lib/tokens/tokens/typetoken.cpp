#include "tokens/tokens/typetoken.hpp"

TypeToken::TypeToken(const TextRange range, std::string_view value) : 
	Token(range, TOKEN_TYPE_VALUE_TYPE),
	valueType(Value::valueType(value)) {}

ValueType TypeToken::getValueType() const {
	return valueType;
}

std::string TypeToken::getStringValue() const {
	return Value::stringType(valueType);
}
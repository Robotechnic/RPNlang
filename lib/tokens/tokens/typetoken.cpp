#include "tokens/tokens/typetoken.hpp"

TypeToken::TypeToken(const TextRange range, std::string_view value, TokenType type) : 
	Token(range, type),
	valueType(stringToType(value)) {}

RPNValueType TypeToken::getValueType() const {
	return this->valueType;
}

std::variant<std::string, ValueType> TypeToken::getListType() const {
	if (this->valueType.index() != 1 || std::get<ValueType>(this->valueType.getType()) != LIST)
		throw std::runtime_error("Cannot get list type of non-list type");
	return this->valueType.getListType();
}

std::string TypeToken::getStringValue() const {
	return this->valueType.name();
}

void TypeToken::setListType(std::variant<std::string, ValueType> listType) {
	if (this->valueType.index() != 1 || std::get<ValueType>(this->valueType.getType()) != LIST)
		throw std::runtime_error("Cannot set list type of non-list type");
	this->valueType.setListType(listType);
}
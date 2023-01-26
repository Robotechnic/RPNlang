#pragma once

#include <string_view>
#include "tokens/token.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"

class TypeToken : public Token {
	public:
		TypeToken(const TextRange range, std::string_view value, TokenType type = TOKEN_TYPE_VALUE_TYPE);

		RPNValueType getValueType() const;
		std::variant<std::string, ValueType> getListType() const;
		std::string getStringValue() const override;

		void setListType(std::variant<std::string, ValueType> listType);

	private:
		RPNValueType valueType;
};
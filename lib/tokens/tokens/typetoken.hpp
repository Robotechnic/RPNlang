#pragma once

#include <string_view>
#include "tokens/token.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"

class TypeToken : public Token {
	public:
		TypeToken(const TextRange range, std::string_view value);

		ValueType getValueType() const;
		std::string getStringValue() const override;

	private:
		ValueType valueType;
};
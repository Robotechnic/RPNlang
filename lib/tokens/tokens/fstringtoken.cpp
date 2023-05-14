#include "tokens/tokens/fstringtoken.hpp"

FStringToken::FStringToken(const TextRange range, std::vector<std::string> parts) : Token(range, TokenType::TOKEN_TYPE_FSTRING), parts(parts) {
	this->stringValue = "";
	std::accumulate(this->parts.begin(), this->parts.end(), this->stringValue, [](const std::string &a, const std::string &b) {
		return a + b;
	});
}

const std::vector<std::string>& FStringToken::getParts() const {
	return this->parts;
}

std::string FStringToken::at(size_t index) const {
	return this->parts.at(index);
}

size_t FStringToken::size() const {
	return this->parts.size();
}

std::string FStringToken::getStringValue() const {
	return this->stringValue;
}
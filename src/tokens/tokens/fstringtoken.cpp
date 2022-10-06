#include "tokens/tokens/fstringtoken.hpp"

FStringToken::FStringToken(const TextRange range, std::vector<std::string> parts) : Token(range, TOKEN_TYPE_FSTRING), parts(parts) {}
std::vector<std::string> FStringToken::getParts() const {
	return this->parts;
}
std::string FStringToken::getStringValue() const {
	std::string result = "";
	std::accumulate(this->parts.begin(), this->parts.end(), result, [](std::string a, std::string b) {
		return a + b;
	});
	return result;
}
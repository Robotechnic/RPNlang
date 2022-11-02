#pragma once

#include <vector>
#include <numeric>
#include <string>
#include "tokens/token.hpp"
#include "tokens/tokentypes.hpp"


class FStringToken : public Token {
	public:
		FStringToken(const TextRange range, std::vector<std::string> parts);
		~FStringToken() override {};
		const std::vector<std::string> &getParts() const;
		std::string getStringValue() const;

	private:
		std::vector<std::string> parts;
};
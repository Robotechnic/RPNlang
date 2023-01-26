#include "tokens/regex.hpp"

matchResult floatMatch(std::string_view str)  {
	if (auto m = ctre::starts_with<"^((?:[\\-])?(?:[0-9]+)?\\.(?:[0-9]+)?)">(str)) {
		if (m.size() > 1)
			return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}

matchResult intMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:[\\-])?[0-9]+)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult binNumMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:0b)([01]+))">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult hexNumMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:0x)([0-9a-fA-F]+))">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult boolMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:true|false))">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult stringMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(?:\")([^\"]*)(?:\")">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult fStringMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^f(?:\")([^\"]*)(?:\")">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult typeMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(int|float|bool|string|function|none|list)(?=[^0-9a-zA-Z_]|$)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult indentBlockMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(\t)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult colonMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([:])">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult arrowMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:\\-\\>))">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult leftBracket(std::string_view str) {
	if (auto m = ctre::starts_with<"^(\\[)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult rightBracket(std::string_view str) {
	if (auto m = ctre::starts_with<"^(\\])">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult expressionSeparatorMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^((?:;))">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult affectTokenMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(=)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult dotMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(\\.)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult capsLiteralMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([A-Z][A-Z0-9_]*)(?=[^0-9a-zA-Z_]|$)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult literalMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([a-z][a-zA-Z0-9]*)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult structMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([A-Z][a-zA-Z0-9]*)(?=[^0-9a-zA-Z_]|$)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult operatorMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([+\\-/*^%])">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult booleanOperatorMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^([<>]=?|==|!=)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult lineSeparatorMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(\\n)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
matchResult commentMatch(std::string_view str) {
	if (auto m = ctre::starts_with<"^(#)">(str)) {
		return std::make_pair(m.get<1>().to_view(), m.size());
	}
	return std::nullopt;
}
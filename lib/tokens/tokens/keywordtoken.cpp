#include "tokens/tokens/keywordtoken.hpp"

KeywordToken::KeywordToken(TextRange range, std::string_view keyword) : Token(range, TokenType::TOKEN_TYPE_KEYWORD) {
	if (keyword == "if") {
		this->keyword = KEYWORD_IF;
	} else if (keyword == "fi") {
		this->keyword = KEYWORD_FI;
	} else if (keyword == "else") {
		this->keyword = KEYWORD_ELSE;
	} else if (keyword == "while") {
		this->keyword = KEYWORD_WHILE;
	} else if (keyword == "elihw") {
		this->keyword = KEYWORD_ELIHW;
	} else if (keyword == "for") {
		this->keyword = KEYWORD_FOR;
	} else if (keyword == "rof") {
		this->keyword = KEYWORD_ROF;
	} else if (keyword == "return") {
		this->keyword = KEYWORD_RETURN;
	} else if (keyword == "continue") {
		this->keyword = KEYWORD_CONTINUE;
	} else if (keyword == "break") {
		this->keyword = KEYWORD_BREAK;
	} else if (keyword == "try") {
		this->keyword = KEYWORD_TRY;
	} else if (keyword == "catch") {
		this->keyword = KEYWORD_CATCH;
	} else if (keyword == "finally") {
		this->keyword = KEYWORD_FINALLY;
	} else if (keyword == "yrt") {
		this->keyword = KEYWORD_YRT;
	} else if (keyword == "fun") {
		this->keyword = KEYWORD_FUN;
	} else if (keyword == "nuf") {
		this->keyword = KEYWORD_NUF;
	} else if (keyword == "struct") {
		this->keyword = KEYWORD_STRUCT;
	} else if (keyword == "tcurts") {
		this->keyword = KEYWORD_TCURTS;
	} else if (keyword == "import") {
		this->keyword = KEYWORD_IMPORT;
	} else if (keyword == "importAs") {
		this->keyword = KEYWORD_IMPORTAS;
	} else if (keyword == "get") {
		this->keyword = KEYWORD_GET;
	} else if (keyword == "funsig") {
		this->keyword = KEYWORD_FUNSIG;
	} else {
		throw std::runtime_error("Invalid keyword");
	}
}

KeywordEnum KeywordToken::getKeyword() const {
	return this->keyword;
}

std::string KeywordToken::keywordString(KeywordEnum keyword) {
	switch (keyword) {
		case KEYWORD_IF:
			return "if";
		case KEYWORD_FI:
			return "fi";
		case KEYWORD_ELSE:
			return "else";
		case KEYWORD_WHILE:
			return "while";
		case KEYWORD_ELIHW:
			return "elihw";
		case KEYWORD_FOR:
			return "for";
		case KEYWORD_ROF:
			return "rof";
		case KEYWORD_RETURN:
			return "return";
		case KEYWORD_CONTINUE:
			return "continue";
		case KEYWORD_BREAK:
			return "break";
		case KEYWORD_TRY:
			return "try";
		case KEYWORD_CATCH:
			return "catch";
		case KEYWORD_FINALLY:
			return "finally";
		case KEYWORD_YRT:
			return "yrt";
		case KEYWORD_FUN:
			return "fun";
		case KEYWORD_NUF:
			return "nuf";
		case KEYWORD_STRUCT:
			return "struct";
		case KEYWORD_TCURTS:
			return "tcurts";
		case KEYWORD_IMPORT:
			return "import";
		case KEYWORD_IMPORTAS:
			return "importas";
		case KEYWORD_GET:
			return "get";
		default:
			throw std::runtime_error("Invalid keyword");
	}
}

std::string KeywordToken::getStringValue() const {
	return KeywordToken::keywordString(this->keyword);
}
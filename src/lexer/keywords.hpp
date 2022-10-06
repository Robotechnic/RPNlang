#pragma once

#include <string>
#include <regex>

const std::regex keywordsRegex("^("
	"fun|"
	"nuf|"
	"if|"
	"fi|"
	"else|"
	"while|"
	"elihw|"
	"for|"
	"rof|"
	"return|"
	"continue|"
	"break|"
	"try|"
	"catch|"
	"finally|"
	"yrt|"
")$");

const std::map<std::string, std::vector<std::string>> blockOpeners= {
	{"fun"    , {"nuf"}},
	{"if"     , {"fi", "else"}},
	{"while"  , {"elihw"}},
	{"for"    , {"rof"}},
	{"try"    , {"yrt", "catch", "finally"}},
	{"else"   , {"fi"}},
	{"catch"  , {"yrt", "finally"}},
	{"finally", {"yrt"}}
};

const std::map<std::string, std::string> blockClosers = {
	{"nuf"    , "fun"},
	{"fi"     , "if"},
	{"elihw"  , "while"},
	{"rof"    , "for"},
	{"yrt"    , "try"},
	{"else"   , "if"},
	{"catch"  , "try"},
	{"finally", "try"}
};
#pragma once

#include <optional>
#include <stack>
#include <string>
#include <unordered_map>

// NOLINTBEGIN
// clang-format off
#include "expressionresult/expressionresult.hpp"
#include "value/valuetypes.hpp"
#include "value/types/numbers/int.hpp"
#include "value/types/struct.hpp"
#include "rpnfunctions/typedef.hpp"
#include "tokens/token.hpp"
#include "tokens/tokens/typetoken.hpp"
#include "tokens/tokens/operatortoken.hpp"
#include "tokens/tokens/valuetoken.hpp"
#include "tokens/tokens/fstringtoken.hpp"
#include "tokens/tokens/keywordtoken.hpp"
#include "codeblocks/line.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/functionblock.hpp"
#include "codeblocks/blockqueue.hpp"
// clang-format on
// NOLINTEND

class FunctionBlock;
class Line;
class CodeBlock;

struct FunctionSignature {
	std::vector<RPNValueType> args;
	RPNValueType returnType;
	bool builtin;
	bool callable = true;
};

struct AnalyzerValueType {
	RPNValueType type;
	TextRange range;
	bool isVariable;
	unsigned int conditionalLevel = 0;
	unsigned int conditionalNextLevel = 0;
	bool isStructMember = false;
	bool isListElement = false;
	bool isFunction = false;

	std::string name() const {
		return type.name();
	};

	int index() const {
		return type.index();
	};
};

using AnalyzerSymbolTable =
	std::unordered_map<std::string, AnalyzerValueType, StringHash, std::equal_to<>>;

class Analyzer final {
  public:
	explicit Analyzer(ContextPtr context);

	void analyze(BlockQueue &blocks, bool entryPoint = false);

	bool hasErrors() const;
	ExpressionResult analyzeErrors() const;

  private:
	ExpressionResult error;
	ContextPtr context;
	unsigned int conditionalLevel = 0;
	bool inFunctionBlock = false;
	RPNValueType currentFunctionReturnType;
	std::vector<unsigned int> nextConditionalLevel = std::vector<unsigned int>{1, 0};

	Line *currentLine{};
	std::stack<AnalyzerValueType> stack;
	std::stack<FunctionBlock *> functionBlocks;
	AnalyzerSymbolTable variables;
	AnalyzerSymbolTable functionVariables;
	std::unordered_map<std::string, FunctionSignature, StringHash> functions;

	AnalyzerValueType &topVariable();
	AnalyzerSymbolTable *getVariables();
	void analyze(Line *line);
	void analyze(CodeBlock *codeblock);
	void analyze(FunctionBlock *functionBlock);
	void analyzeFunctionsBody();
	void checkRemainingCount();
	void analyzeOperator(const OperatorToken *token);
	void analyzeFString(const FStringToken *token);
	void analyzeFunctionCall(FunctionSignature function, Token *token);
	void analyzeFunctionCall(Token *token);
	void analyzeAssignment(const Token *token);
	void analyzeTypeCast(const TypeToken *token);
	void analyzeListCreation(const TypeToken *token);
	void analyzeStructCreation(const Token *token);
	void analyzeKeyword(const KeywordToken *token);
	void checkKeywordLine(const KeywordToken *token, bool restaureStack, bool strict = true);
	void analyzeImport(const KeywordToken *token);
	void analyzeImportAs(const KeywordToken *token);
	void analyzeFunctionSignature(const KeywordToken *token);
	void analyzeReturn(const KeywordToken *token);
	void analyzePath(Token *path, bool addToStack = true);
	void analyzeStructAccess(const Token *token);
	void analyzeGet(const Token *token);

	static bool isBinaryOperator(OperatorToken::OperatorTypes operatorType);
	static bool isComparisonOperator(OperatorToken::OperatorTypes operatorType);
	static std::optional<ValueType> getOperatorType(ValueType left, ValueType right,
													const OperatorToken *operatorToken);
};

std::ostream &operator<<(std::ostream &stream, const AnalyzerValueType &valueType);

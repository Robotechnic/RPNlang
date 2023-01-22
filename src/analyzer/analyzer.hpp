#pragma once

#include <string>
#include <stack>
#include <unordered_map>
#include <optional>

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


class FunctionBlock;
class Line;
class CodeBlock;

struct AnalyzerValueType {
	RPNValueType type;
	TextRange range;
	bool isVariable;
	int conditionalLevel;
	int conditionalNextLevel;
};

class Analyzer final {
	public:
		Analyzer(ContextPtr context);

		void analyze(BlockQueue &blocks, bool entryPoint = false);
		
		bool hasErrors() const;
		ExpressionResult analyzeErrors() const;

	private:
		ExpressionResult error;
		ContextPtr context;
		int conditionalLevel;
		bool inFunctionBlock;
		int nextConditionalLevel;

		Line *currentLine;
		std::stack<AnalyzerValueType> stack;
		std::stack<FunctionBlock*> functionBlocks;
		std::unordered_map<std::string, AnalyzerValueType> variables;
		std::unordered_map<std::string, AnalyzerValueType> functionVariables;
		std::unordered_map<std::string, std::pair<std::vector<RPNValueType>, RPNValueType>> functions;
		
		AnalyzerValueType& topVariable();
		std::unordered_map<std::string, AnalyzerValueType> *getVariables();
		void analyze(Line *line);
		void analyze(CodeBlock *codeblock);
		void analyze(FunctionBlock *functionBlock);
		void analyzeFunctionsBody();
		void checkRemainingCount();
		void analyzeOperator(const OperatorToken *token);
		void analyzeFString(const FStringToken *token);
		void analyzeFunctionCall(std::pair<std::vector<RPNValueType>, RPNValueType> function, const Token *token);
		void analyzeFunctionCall(Token *token);
		void analyzeAssignment(const Token *token);
		void analyzeTypeCast(const TypeToken *token);
		void analyzeListCreation(const TypeToken *token);
		void analyzeStructCreation(const Token *token);
		void analyzeKeyword(const KeywordToken *token);
		void checkKeywordLine(const KeywordToken *token);
		void analyzeImport(const KeywordToken *token);
		void analyzeImportAs(const KeywordToken *token);
		void analyzePath(Token *path, bool addToStack = true);

		static bool isBinaryOperator(OperatorToken::OperatorTypes operatorType);
		static bool isComparisonOperator(OperatorToken::OperatorTypes operatorType);
		static std::optional<ValueType> getOperatorType(ValueType left, ValueType right, const OperatorToken *operatorToken);
};



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

class FunctionBlock;
class Line;
class CodeBlock;

struct AnalyzerValueType {
	RPNValueType type;
	TextRange range;
	bool isVariable;
};

class Analyzer final {
	public:
		Analyzer(ContextPtr context);

		void analyze(Line *line);
		void analyze(CodeBlock *codeblock);
		void analyseFunctionsBody();

		bool hasErrors() const;
		ExpressionResult analyzeErrors() const;
		RPNValueType getLastType() const;

	private:
		ExpressionResult error;
		ContextPtr context;
		RPNValueType lastType;

		Line *currentLine;
		std::stack<AnalyzerValueType> stack;
		std::queue<CodeBlock*> functionBlocks;
		std::unordered_map<std::string, AnalyzerValueType> variables;
		std::unordered_map<std::string, std::pair<RPNFunctionArgTypes, RPNValueType>> functions;

		AnalyzerValueType& topVariable();
		void analyze(FunctionBlock *functionBlock);
		void analyzeOperator(const OperatorToken *token);
		void analyzeFString(const FStringToken *token);
		void analyzeFunctionCall(std::pair<RPNFunctionArgTypes, RPNValueType> function, const Token *token);
		void analyzeFunctionCall(const Token *token);
		void analyzeAssignment(const Token *token);
		void analyzeTypeCast(const TypeToken *token);
		void analyzeListCreation(const TypeToken *token);
		void analyzeStructCreation(const Token *token);

		static bool isBinaryOperator(OperatorToken::OperatorTypes operatorType);
		static bool isComparisonOperator(OperatorToken::OperatorTypes operatorType);
		static std::optional<ValueType> getOperatorType(ValueType left, ValueType right, const OperatorToken *operatorToken);
};


#include "codeblocks/line.hpp"
#include "codeblocks/codeblock.hpp"
#include "codeblocks/functionblock.hpp"
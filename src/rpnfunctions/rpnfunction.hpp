#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "value/valuetype.hpp"

class ExpressionResult;
class Value;
class Context;
class TextRange;

typedef std::vector<Value> RPNFunctionArgs;
typedef std::tuple<ExpressionResult, Value> RPNFunctionResult;


class RPNFunction {
	public:
		RPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> parameterTypes,
			ValueType returnType
		);
		virtual ~RPNFunction();

		virtual RPNFunctionResult call(
			RPNFunctionArgs args,
			Context *context
		) const;

		int getArgumentsCount() const;
		std::string getName() const;
		virtual TextRange getRange() const;

	protected:
		ExpressionResult checkTypes(const RPNFunctionArgs &args, const Context *context) const;
		ExpressionResult checkArgs(const RPNFunctionArgs &args, const Context *context) const;
		

		std::string name;
		std::vector<std::string> argsName;
		std::vector<ValueType> parameterTypes;
		ValueType returnType;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);


#include "tokens/token.hpp"
#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/value.hpp"
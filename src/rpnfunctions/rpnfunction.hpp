#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>

class Value;

#include "value/value.hpp"
#include "value/valuetype.hpp"
#include "tokens/token.hpp"
#include "expressionresult/expressionresult.hpp"

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

		virtual RPNFunctionResult call(
			RPNFunctionArgs args,
			std::map<std::string, Value> variables
		) const;

		int getArgumentsCount() const;
		std::string getName() const;
		virtual TextRange getRange() const;

	protected:
		ExpressionResult checkTypes(const RPNFunctionArgs &args) const;
		ExpressionResult checkArgs(const RPNFunctionArgs &args) const;
		

		std::string name;
		std::vector<std::string> argsName;
		std::vector<ValueType> parameterTypes;
		ValueType returnType;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);
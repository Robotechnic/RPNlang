#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "tokens/token.hpp"
#include "value/valuetypes.hpp"
#include "rpnfunctions/typedef.hpp"

class ExpressionResult;
class TextRange;


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

#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/value.hpp"
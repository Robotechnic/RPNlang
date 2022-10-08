#pragma once

#include <string>
#include <vector>
#include "rpnfunctions/typedef.hpp"
#include "expressionresult/expressionresult.hpp"
#include "context/context.hpp"
#include "value/valuetypes.hpp"
#include "value/value.hpp"
#include "value/types/none.hpp"


class RPNFunction {
	public:
		RPNFunction(
			std::string name,
			std::vector<std::string> argsName, std::vector<ValueType> argsTypes,
			ValueType returnType
		);
		virtual ~RPNFunction();

		virtual RPNFunctionResult call(
			RPNFunctionArgs args,
			Context *context
		) const;

		int getArgumentsCount() const;
		std::string getName() const;

	protected:
		ExpressionResult checkTypes(const RPNFunctionArgs &args, const Context *context) const;
		ExpressionResult checkArgs(const RPNFunctionArgs &args, const Context *context) const;
		

		std::string name;
		std::vector<std::string> argsName;
		std::vector<ValueType> argsTypes;
		ValueType returnType;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);
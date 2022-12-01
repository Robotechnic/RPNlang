#pragma once

#include <string>
#include <vector>
#include "rpnfunctions/typedef.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "value/types/none.hpp"
#include "context/context.hpp"

class RPNFunction {
	public:
		RPNFunction(
			const std::string &name,
			const std::vector<std::string> &argsName,
			const std::vector<ValueType> &argsTypes,
			const ValueType &returnType
		);
		virtual ~RPNFunction();

		virtual RPNFunctionResult call(
			const RPNFunctionArgs &args,
			const TextRange &range,
			Context *context
		) const;

		size_t getArgumentsCount() const;
		std::string getName() const;
		TextRange getRange() const;

	protected:
		ExpressionResult checkTypes(const RPNFunctionArgs &args, const Context *context) const;
		ExpressionResult checkArgs(const RPNFunctionArgs &args, const Context *context) const;
		

		std::string name;
		std::vector<std::string> argsName;
		std::vector<ValueType> argsTypes;
		ValueType returnType;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);
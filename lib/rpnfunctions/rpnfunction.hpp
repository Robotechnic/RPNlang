#pragma once

#include <string>
#include <vector>
#include "rpnfunctions/typedef.hpp"
#include "expressionresult/expressionresult.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include "value/types/none.hpp"
#include "value/types/struct.hpp"
#include "context/context.hpp"

class RPNFunction {
	public:
		RPNFunction(
			std::string_view name,
			const RPNFunctionArgs &arguments,
			const RPNValueType &returnType
		);
		virtual ~RPNFunction();

		virtual RPNFunctionResult call(
			RPNFunctionArgsValue &args,
			const TextRange &range,
			ContextPtr context
		) const;

		size_t getArgumentsCount() const;
		std::string getName() const;
		TextRange getRange() const;

		RPNValueType getReturnType() const;
		RPNFunctionArgs getArgs() const;

	protected:
		std::string name;
		RPNFunctionArgs arguments;
		RPNValueType returnType;
};

std::ostream& operator<<(std::ostream& os, const RPNFunction& function);
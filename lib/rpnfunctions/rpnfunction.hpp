#pragma once

#include "context/context.hpp"
#include "expressionresult/expressionresult.hpp"
#include "rpnfunctions/functionsignature.hpp"
#include "rpnfunctions/typedef.hpp"
#include "value/types/none.hpp"
#include "value/types/struct.hpp"
#include "value/value.hpp"
#include "value/valuetypes.hpp"
#include <string>
#include <vector>

class RPNFunction {
  public:
	RPNFunction(std::string_view name, const RPNFunctionArgs &arguments,
				const RPNValueType &returnType);
	virtual ~RPNFunction();

	virtual RPNFunctionResult call(RPNFunctionArgsValue &args, const TextRange &range,
								   ContextPtr context) const;

	size_t getArgumentsCount() const;
	std::string getName() const;
	TextRange getRange() const;

	RPNValueType getReturnType() const;
	RPNFunctionArgs getArgs() const;

	FunctionSignature getSignature() const;

  protected:
	std::string name;
	RPNFunctionArgs arguments;
	RPNValueType returnType;
};

std::ostream &operator<<(std::ostream &os, const RPNFunction &function);
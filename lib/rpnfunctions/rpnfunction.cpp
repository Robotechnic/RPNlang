#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string_view name,
	const RPNFunctionArgs &arguments,
	const RPNValueType &returnType
):
	name(name),
	arguments(arguments),
	returnType(returnType)
{}

RPNFunction::~RPNFunction() {}

/**
 * @brief Take the function body and run it, must be overriden
 * 
 * @param args arguments of the function
 * @param context the parent context
 * @return RPNFunctionResult If the function was executed successfully and value if it is the case
 */
RPNFunctionResult RPNFunction::call(
	RPNFunctionArgsValue &args,
	const TextRange &range,
	ContextPtr context
) const {
	TextRange errorRange = range;
	if (args.size() != 0) {
		errorRange.merge(args[0]->getRange());
		if (args.size() > 1) {
			errorRange.merge(args.back()->getRange());
		}
	}
	ContextPtr functionContext = std::make_shared<Context>(this->name, "", context, CONTEXT_TYPE_FUNCTION);
	return ExpressionResult(
		"Function is not callable", 
		range,
		functionContext
	);
}

size_t RPNFunction::getArgumentsCount() const {
	return this->arguments.size();
}

std::string RPNFunction::getName() const {
	return this->name;
}

TextRange RPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}

RPNValueType RPNFunction::getReturnType() const {
	return this->returnType;
}

RPNFunctionArgs RPNFunction::getArgs() const {
	return this->arguments;
}
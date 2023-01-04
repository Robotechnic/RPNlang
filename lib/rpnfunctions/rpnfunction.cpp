#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string_view name,
	const std::vector<std::string> &argsName,
	const std::vector<ValueType> &argsTypes,
	const ValueType &returnType
):
	name(name),
	argsName(argsName),
	argsTypes(argsTypes),
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
	RPNFunctionArgs &args,
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
	return std::make_pair(ExpressionResult(
			"Function is not callable", 
			range,
			functionContext
		),
		None::empty()
	);
}

/**
 * @brief check if all provided arguments have the correct types or if they can be converted to the correct types
 * 
 * @param args the arguments provided by the user
 * @return ExpressionResult if the arguments are correct
 */
ExpressionResult RPNFunction::checkTypes(RPNFunctionArgs &args, const ContextPtr &context) const {
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i]->getType() == this->argsTypes[i] || this->argsTypes[i] == ANY) continue;
		
		if (!args[i]->isCastableTo(this->argsTypes[i])) {
			return ExpressionResult(
				"Function call argument type mismatch, expected " + 
				std::string(Value::stringType(this->argsTypes[i])) + 
				" but got " + args[i]->getStringType(),
				args[i]->getRange(),
				context
			);
		}
	}
	return ExpressionResult();
}

size_t RPNFunction::getArgumentsCount() const {
	return this->argsName.size();
}

std::string RPNFunction::getName() const {
	return this->name;
}

TextRange RPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}
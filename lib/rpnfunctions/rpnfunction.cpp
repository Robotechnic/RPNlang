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
	const RPNFunctionArgs &args,
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
	return std::make_tuple(ExpressionResult(
			"Function is not callable", 
			range,
			functionContext
		),
		None::empty()
	);
}

/**
 * @brief check if there is enough arguments and if the provided arguments have the correct types
 * 
 * @param args arguments provided by the user
 * @return ExpressionResult if the arguments are correct
 */
ExpressionResult RPNFunction::checkArgs(const RPNFunctionArgs &args, const ContextPtr &context) const {
	if (args.size() != this->argsName.size()) {
		TextRange range(0, 0, 0);
		if (args.size() != 0) {
			range = args[0]->getRange();
			if (args.size() > 1) {
				range.merge(args[args.size() - 1]->getRange());
			}
		}

		return ExpressionResult(
			"Function call arguments count mismatch," + std::to_string(this->argsName.size()) + 
			" arguents expected but got " + std::to_string(args.size()) + " arguments",
			range,
			context
		);
	}

	ExpressionResult result = this->checkTypes(args, context);
	if (result.error()) return result;

	return ExpressionResult();
}

/**
 * @brief check if all provided arguments have the correct types or if they can be converted to the correct types
 * 
 * @param args the arguments provided by the user
 * @return ExpressionResult if the arguments are correct
 */
ExpressionResult RPNFunction::checkTypes(const RPNFunctionArgs &args, const ContextPtr &context) const {
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i]->getType() == this->argsTypes[i]) continue;
		
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
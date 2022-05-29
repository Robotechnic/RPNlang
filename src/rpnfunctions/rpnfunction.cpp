#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string name,
	std::vector<std::string> argsName,
	std::vector<ValueType> parameterTypes,
	ValueType returnType
):
	name(name),
	argsName(argsName),
	parameterTypes(parameterTypes),
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
	RPNFunctionArgs args,
	Context *context
) const {
	TextRange range(0, 0, 0);
	if (args.size() != 0) {
		range = args[0].getRange();
		if (args.size() > 1) {
			range.columnEnd = args[args.size() - 1].getRange().columnEnd;
		}
	}
	context->setChild(new Context(this->name, context, CONTEXT_TYPE_FUNCTION));
	return std::make_tuple(ExpressionResult(
			"Function is not callable", 
			range,
			context->getChild()
		),
		Value()
	);
}

/**
 * @brief check if there is enough arguments and if the provided arguments have the correct types
 * 
 * @param args arguments provided by the user
 * @return ExpressionResult if the arguments are correct
 */
ExpressionResult RPNFunction::checkArgs(const RPNFunctionArgs &args, const Context *context) const{
	if (args.size() != this->argsName.size()) {
		TextRange range(0, 0, 0);
		if (args.size() != 0) {
			range = args[0].getRange();
			if (args.size() > 1) {
				range.merge(args[args.size() - 1].getRange());
			}
		}

		return ExpressionResult(
			"Function call arguments count mismatch," + 
			std::to_string(this->argsName.size()) + 
			" arguents expected but got " + 
			std::to_string(args.size()) + 
			" arguments",
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
ExpressionResult RPNFunction::checkTypes(const RPNFunctionArgs &args, const Context *context) const{
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i].getType() != this->parameterTypes[i]) {
			if (!args[i].isCastableTo(this->parameterTypes[i])) {
				return ExpressionResult(
					"Function call argument type mismatch, expected " + 
					Value::stringType(this->parameterTypes[i]) + 
					" but got " + Value::stringType(args[i].getType()),
					args[i].getRange(),
					context
				);
			}
		}
	}
	return ExpressionResult();
}

int RPNFunction::getArgumentsCount() const {
	return this->argsName.size();
}

std::string RPNFunction::getName() const {
	return this->name;
}

TextRange RPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}
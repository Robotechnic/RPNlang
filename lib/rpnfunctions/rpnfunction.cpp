#include "rpnfunctions/rpnfunction.hpp"

RPNFunction::RPNFunction(
	std::string_view name,
	const std::vector<std::string> &argsName,
	const RPNFunctionArgTypes &argsTypes,
	const ValueType &returnType
):
	name(name),
	argsName(argsName),
	argsTypes(argsTypes),
	returnType(returnType)
{
	if (argsName.size() != argsTypes.size()) {
		throw std::runtime_error("RPNFunction: argsName and argsTypes must have the same size");
	}
}

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
		if (const auto structName (std::get_if<std::string>(&this->argsTypes[i])); structName) {
			if (args[i]->getType() != STRUCT) {
				return ExpressionResult(
					"Function call argument type mismatch, expected struct" +
					*structName + " but got " + args[i]->getStringType(),
					args[i]->getRange(),
					context
				);
			}
			if (static_cast<Struct*>(args[i])->getStructName() != *structName) {
				return ExpressionResult(
					"Function call argument type mismatch, expected struct " +
					*structName + " but got struct " + std::string(static_cast<Struct*>(args[i])->getStructName()),
					args[i]->getRange(),
					context
				);
			}
			continue;
		}
		
		ValueType type = std::get<ValueType>(this->argsTypes[i]);
		if (args[i]->getType() == type || type == ANY) continue;
		if (!args[i]->isCastableTo(type)) {
			return ExpressionResult(
				"Function call argument type mismatch, expected " + 
				std::string(Value::stringType(type)) + 
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
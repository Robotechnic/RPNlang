#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			std::function<RPNFunctionResult(RPNFunctionArgs&, const Context&)> function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {}

RPNFunctionResult BuiltinRPNFunction::call(
	RPNFunctionArgs args,
	const Context &context
) const {
	Context functionContext(this->name, &context, CONTEXT_TYPE_FUNCTION);

	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) return std::make_tuple(result, Value());
	return this->function(args, functionContext);
}

TextRange BuiltinRPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}

// until modules are implemented
const std::map<std::string, BuiltinRPNFunction> BuiltinRPNFunction::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{"value"},
		{ValueType::STRING},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context context) {
			std::cout << args[0].getStringValue();
			return std::make_tuple(ExpressionResult(), Value());
		}
	)},
	{"input", BuiltinRPNFunction(
		"input",
		{"value"},
		{ValueType::STRING},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context context) {
			std::cout<<args[0].getStringValue();
			std::string input;
			std::getline(std::cin, input);
			return std::make_tuple(ExpressionResult(), Value(input, 0, 0));
		}
	)},
	{"len", BuiltinRPNFunction(
		"len",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value((int)args[0].getStringValue().size(), 0, 0));
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{"value", "start", "length"},
		{ValueType::STRING, ValueType::INT, ValueType::INT},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context context) {
			std::string value = args[0].getStringValue();
			int start = args[1].getIntValue();
			int length = args[2].getIntValue();
			return std::make_tuple(ExpressionResult(), Value(value.substr(start, length), 0, 0));
		}
	)},
	{"int", BuiltinRPNFunction(
		"int",
		{"value"},
		{ValueType::FLOAT},
		ValueType::INT,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(static_cast<int>(args[0].getFloatValue()), 0, 0));
		}
	)},
	{"float", BuiltinRPNFunction(
		"float",
		{"value"},
		{ValueType::INT},
		ValueType::FLOAT,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(static_cast<float>(args[0].getIntValue()), 0, 0));
		}
	)},
	{"isNumber", BuiltinRPNFunction(
		"isNumber",
		{"value"},
		{ValueType::STRING},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(args[0].isNumber(), 0, 0));
		}
	)},
	{"toInt", BuiltinRPNFunction(
		"toInt",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context context) {
			if (args[0].isNumber()) {
				return std::make_tuple(ExpressionResult(), Value(args[0].getIntValue(), 0, 0));
			}
			return std::make_tuple(
				ExpressionResult(
					"Cannot convert '" + args[0].getStringValue() + "' to int",
					args[0].getRange(),
					context
				),
				Value()
			);
		}
	)},
	{"toFloat", BuiltinRPNFunction(
		"toFloat",
		{"value"},
		{ValueType::STRING},
		ValueType::FLOAT,
		[](RPNFunctionArgs args, Context context) {
			if (args[0].isNumber()) {
				return std::make_tuple(ExpressionResult(), Value(args[0].getFloatValue(), 0, 0));
			}
			return std::make_tuple(
				ExpressionResult(
					"Cannot convert '" + args[0].getStringValue() + "' to float",
					args[0].getRange(),
					context
				),
				Value()
			);
		}
	)},
	{"and", BuiltinRPNFunction(
		"and",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(args[0].getBoolValue() && args[1].getBoolValue(), 0, 0));
		}
	)},
	{"or", BuiltinRPNFunction(
		"or",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(args[0].getBoolValue() || args[1].getBoolValue(), 0, 0));
		}
	)},
	{"not", BuiltinRPNFunction(
		"not",
		{"value"},
		{ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(!args[0].getBoolValue(), 0, 0));
		}
	)},
	{"ord", BuiltinRPNFunction(
		"ord",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context context) {
			if (args[0].getStringValue().size() != 1) return std::make_tuple(
				ExpressionResult(
					"string must have length of 1", args[0].getRange(),
					context
				),
				Value()
			);
			return std::make_tuple(ExpressionResult(), Value((int)args[0].getStringValue()[0], 0, 0));
		}
	)},
	{"chr", BuiltinRPNFunction(
		"chr",
		{"value"},
		{ValueType::INT},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context context) {
			return std::make_tuple(ExpressionResult(), Value(std::string(1, (char)args[0].getIntValue()), 0, 0));
		}
	)}
};
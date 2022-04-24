#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			std::function<RPNFunctionResult(RPNFunctionArgs&)> function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {}

RPNFunctionResult BuiltinRPNFunction::call(
	RPNFunctionArgs args,
	std::map<std::string, Value> variables
) const {
	ExpressionResult result = this->checkArgs(args);
	if (result.error()) return std::make_tuple(result, Value());
	return this->function(args);
}

TextRange BuiltinRPNFunction::getRange() const {
	return TextRange(0, 0, 0);
}


const std::map<std::string, BuiltinRPNFunction> BuiltinRPNFunction::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{"value"},
		{ValueType::STRING},
		ValueType::NONE,
		[](RPNFunctionArgs args) {
			std::cout << args[0].getStringValue() << std::endl;
			return std::make_tuple(ExpressionResult(), Value());
		}
	)},
	{"input", BuiltinRPNFunction(
		"input",
		{"value"},
		{ValueType::STRING},
		ValueType::STRING,
		[](RPNFunctionArgs args) {
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
		[](RPNFunctionArgs args) {
			return std::make_tuple(ExpressionResult(), Value((int)args[0].getStringValue().size(), 0, 0));
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{"value", "start", "length"},
		{ValueType::STRING, ValueType::INT, ValueType::INT},
		ValueType::STRING,
		[](RPNFunctionArgs args) {
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
		[](RPNFunctionArgs args) {
			return std::make_tuple(ExpressionResult(), Value(static_cast<int>(args[0].getFloatValue()), 0, 0));
		}
	)},
	{"float", BuiltinRPNFunction(
		"float",
		{"value"},
		{ValueType::INT},
		ValueType::FLOAT,
		[](RPNFunctionArgs args) {
			return std::make_tuple(ExpressionResult(), Value(static_cast<float>(args[0].getIntValue()), 0, 0));
		}
	)}
};
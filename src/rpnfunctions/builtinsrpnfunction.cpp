#include "rpnfunctions/builtinsrpnfunction.hpp"

BuiltinRPNFunction::BuiltinRPNFunction(
			std::string name,
			std::vector<std::string> argsName,
			std::vector<ValueType> argumentsTypes,
			ValueType returnType,
			BuiltinRPNFunctionType function
		) : RPNFunction(name, argsName, argumentsTypes, returnType), function(function) {}

BuiltinRPNFunction::~BuiltinRPNFunction() {}

RPNFunctionResult BuiltinRPNFunction::call(
	RPNFunctionArgs args,
	Context *context
) const {
	context->setChild(new Context(this->name, context, CONTEXT_TYPE_FUNCTION));

	ExpressionResult result = this->checkArgs(args, context);
	if (result.error()) return std::make_tuple(result, nullptr);
	RPNFunctionResult functionResult = this->function(args, context->getChild());

	return functionResult;
}

TextRange BuiltinRPNFunction::getRange() const {
	return TextRange();
}

const std::map<std::string, BuiltinRPNFunction> BuiltinRPNFunction::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{"value"},
		{ValueType::STRING},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context *context) {
			std::cout << args[0]->getStringValue();
			return std::make_tuple(ExpressionResult(), nullptr);
		}
	)},
	{"input", BuiltinRPNFunction(
		"input",
		{"value"},
		{ValueType::STRING},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context *context) {
			std::cout<<args[0]->getStringValue();
			std::string input;
			std::cin>>input;
			return std::make_tuple(ExpressionResult(), new String(input, TextRange()));
		}
	)},
	{"len", BuiltinRPNFunction(
		"len",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(ExpressionResult(), new Int(args[0]->getStringValue().size(), TextRange()));
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{"value", "start", "length"},
		{ValueType::STRING, ValueType::INT, ValueType::INT},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context *context) {
			std::string value = args[0]->getStringValue();
			int start = static_cast<Int *>(args[1])->getValue();
			int length = static_cast<Int *>(args[2])->getValue();
			return std::make_tuple(ExpressionResult(), new String(value.substr(start, length), TextRange()));
		}
	)},
	{"isNumber", BuiltinRPNFunction(
		"isNumber",
		{"value"},
		{ValueType::STRING},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(ExpressionResult(), new Bool(args[0]->isNumber(), TextRange()));
		}
	)},
	{"toInt", BuiltinRPNFunction(
		"toInt",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context *context) {
			if (std::regex_match(args[0]->getStringValue(), intRegex)) {
				return std::make_tuple(ExpressionResult(), new Int(std::stoi(args[0]->getStringValue()), TextRange()));
			}

			return std::make_tuple(
				ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to int",
					args[0]->getRange(),
					context
				),
				&Int::emptyInt
			);
		}
	)},
	{"toFloat", BuiltinRPNFunction(
		"toFloat",
		{"value"},
		{ValueType::STRING},
		ValueType::FLOAT,
		[](RPNFunctionArgs args, Context *context) {
			if (std::regex_match(args[0]->getStringValue(), floatRegex)) {
				return std::make_tuple(ExpressionResult(), new Float(std::stof(args[0]->getStringValue()), TextRange()));
			}
			return std::make_tuple(
				ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to float",
					args[0]->getRange(),
					context
				),
				&Float::emptyFloat
			);
		}
	)},
	{"toString", BuiltinRPNFunction(
		"toString",
		{"value"},
		{ValueType::FLOAT},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(ExpressionResult(), new String(args[0]->getStringValue(), TextRange()));
		}
	)},
	{"and", BuiltinRPNFunction(
		"and",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(
				ExpressionResult(), 
				new Bool(
					static_cast<Bool*>(args[0])->getValue() && static_cast<Bool*>(args[1])->getValue(),
					TextRange()
				)
			);
		}
	)},
	{"or", BuiltinRPNFunction(
		"or",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(
				ExpressionResult(),
				new Bool(
					static_cast<Bool*>(args[0])->getValue() || static_cast<Bool*>(args[1])->getValue(), 
					TextRange()
				)
			);
		}
	)},
	{"not", BuiltinRPNFunction(
		"not",
		{"value"},
		{ValueType::BOOL},
		ValueType::BOOL,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(
				ExpressionResult(), 
				new Bool(
					!static_cast<Bool*>(args[0])->getValue(),
					TextRange()
				)
			);
		}
	)},
	{"ord", BuiltinRPNFunction(
		"ord",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](RPNFunctionArgs args, Context *context) {
			if (args[0]->getStringValue().size() != 1) return std::make_tuple(
				ExpressionResult(
					"string must have length of 1", args[0]->getRange(),
					context
				),
				&Int::emptyInt
			);
			return std::make_tuple(
				ExpressionResult(),
				new Int(
					static_cast<int64_t>(args[0]->getStringValue()[0]), 
					TextRange()
				)
			);
		}
	)},
	{"chr", BuiltinRPNFunction(
		"chr",
		{"value"},
		{ValueType::INT},
		ValueType::STRING,
		[](RPNFunctionArgs args, Context *context) {
			return std::make_tuple(
				ExpressionResult(), 
				new String(
					std::string(1, static_cast<char>(static_cast<Int*>(args[0])->getValue())), 
					TextRange()
				)
			);
		}
	)},
	{"exit", BuiltinRPNFunction(
		"exit",
		{"value"},
		{ValueType::INT},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context *context) {
			exit(static_cast<Int*>(args[0])->getValue());
			// Should never reach this point
			return std::make_tuple(ExpressionResult(), nullptr);
		}
	)},
	{"assert",BuiltinRPNFunction(
		"assert",
		{"value"},
		{ValueType::BOOL},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context *context) {
			if (!static_cast<Bool *>(args[0])->getValue())
				return std::make_tuple(
					ExpressionResult(
						"Assertion failed", 
						args[0]->getRange(), 
						context->getParent()
					), 
					&None::emptyNone
				);
			
			return std::make_tuple(ExpressionResult(), new None(TextRange()));
		}
	)},
	{"import", BuiltinRPNFunction(
		"import",
		{"path"},
		{ValueType::STRING},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context *context) {
			std::string path = args[0]->getStringValue();
			if (path.size() == 0) {
				return std::make_tuple(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					nullptr
				);
			}
			return std::make_tuple(
				Module::addModule(path, extractFileName(path), args[0]->getRange(), context->getParent()),
				nullptr
			);
		}
	)},
	{"importAs", BuiltinRPNFunction(
		"importAs",
		{"path", "name"},
		{ValueType::STRING, ValueType::STRING},
		ValueType::NONE,
		[](RPNFunctionArgs args, Context *context) {
			std::string path = args[0]->getStringValue();
			if (path.size() == 0) {
				return std::make_tuple(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					nullptr
				);
			}
			return std::make_tuple(
				Module::addModule(path, args[1]->getStringValue(), args[0]->getRange(), context->getParent()),
				nullptr
			);
		}
	)}
};
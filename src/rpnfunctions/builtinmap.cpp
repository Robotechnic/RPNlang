#include "rpnfunctions/builtinmap.hpp"

const std::unordered_map<std::string, BuiltinRPNFunction> builtins::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{"value"},
		{ValueType::STRING},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::cout << args[0]->getStringValue();
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	)},
	{"input", BuiltinRPNFunction(
		"input",
		{"value"},
		{ValueType::STRING},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::cout<<args[0]->getStringValue();
			std::string input;
			std::getline(std::cin, input);
			return std::make_tuple(ExpressionResult(), new String(input, range, true));
		}
	)},
	{"len", BuiltinRPNFunction(
		"len",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(ExpressionResult(), new Int(args[0]->getStringValue().size(), range, true));
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{"value", "start", "length"},
		{ValueType::STRING, ValueType::INT, ValueType::INT},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string value = args[0]->getStringValue();
			int start = static_cast<Int *>(args[1])->getValue();
			int length = static_cast<Int *>(args[2])->getValue();
			return std::make_tuple(ExpressionResult(), new String(value.substr(start, length), range, true));
		}
	)},
	{"isNumber", BuiltinRPNFunction(
		"isNumber",
		{"value"},
		{ValueType::STRING},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string value = args[0]->getStringValue();
			bool isNumber = value == "true";
			isNumber |= value == "false";
			isNumber |= std::regex_match(value, floatRegex);
			isNumber |= std::regex_match(value, intRegex);
			return std::make_tuple(ExpressionResult(), new Bool(isNumber, range, true));
		}
	)},
	{"toInt", BuiltinRPNFunction(
		"toInt",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			RPNFunctionResult result = std::make_pair(ExpressionResult(), nullptr);
			if (std::regex_match(args[0]->getStringValue(), intRegex)) {
				std::get<1>(result) = new Int(std::stoi(args[0]->getStringValue()), range, true);
			} else {
				std::get<0>(result) = ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to int",
					args[0]->getRange(),
					context
				);
				std::get<1>(result) = Int::empty();
			}
			return result;
		}
	)},
	{"toFloat", BuiltinRPNFunction(
		"toFloat",
		{"value"},
		{ValueType::STRING},
		ValueType::FLOAT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			RPNFunctionResult result = std::make_pair(ExpressionResult(), nullptr);
			if (std::regex_match(args[0]->getStringValue(), floatRegex) || 
				std::regex_match(args[0]->getStringValue(), intRegex)
			) {
				std::get<1>(result) = new Float(std::stof(args[0]->getStringValue()), range, true);
			} else {
				std::get<0>(result) = ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to float",
					args[0]->getRange(),
					context
				);
				std::get<1>(result) = Float::empty();
			}
			return result;
		}
	)},
	{"toString", BuiltinRPNFunction(
		"toString",
		{"value"},
		{ValueType::FLOAT},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(ExpressionResult(), new String(args[0]->getStringValue(), range, true));
		}
	)},
	{"and", BuiltinRPNFunction(
		"and",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(
				ExpressionResult(), 
				new Bool(
					static_cast<Bool*>(args[0])->getValue() && static_cast<Bool*>(args[1])->getValue(),
					range,
					true
				)
			);
		}
	)},
	{"or", BuiltinRPNFunction(
		"or",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(
				ExpressionResult(),
				new Bool(
					static_cast<Bool*>(args[0])->getValue() || static_cast<Bool*>(args[1])->getValue(), 
					range,
					true
				)
			);
		}
	)},
	{"not", BuiltinRPNFunction(
		"not",
		{"value"},
		{ValueType::BOOL},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(
				ExpressionResult(), 
				new Bool(
					!static_cast<Bool*>(args[0])->getValue(),
					range,
					true
				)
			);
		}
	)},
	{"ord", BuiltinRPNFunction(
		"ord",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::tuple<ExpressionResult, Value*> result = std::make_pair(ExpressionResult(), nullptr);
			if (args[0]->getStringValue().size() != 1) {
				std::get<0>(result) = ExpressionResult(
					"string must have length of 1", args[0]->getRange(),
					context
				);
				std::get<1>(result) = Int::empty();
			} else
				std::get<1>(result) = new Int(static_cast<int64_t>(args[0]->getStringValue()[0]), range, true);

			return result;
		}
	)},
	{"chr", BuiltinRPNFunction(
		"chr",
		{"value"},
		{ValueType::INT},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_tuple(
				ExpressionResult(), 
				new String(
					std::string(1, static_cast<char>(static_cast<Int*>(args[0])->getValue())), 
					range,
					true
				)
			);
		}
	)},
	{"exit", BuiltinRPNFunction(
		"exit",
		{"value"},
		{ValueType::INT},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			exit(static_cast<Int*>(args[0])->getValue());
			// Should never reach this point
			return std::make_tuple(ExpressionResult(), None::empty());
		}
	)},
	{"assert",BuiltinRPNFunction(
		"assert",
		{"value"},
		{ValueType::BOOL},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			ExpressionResult result;

			if (!static_cast<Bool *>(args[0])->getValue()) {
				result = ExpressionResult(
					"Assertion failed",
					args[0]->getRange(),
					context->getParent()
				);
			}

			return std::make_tuple(result, None::empty());
		}
	)},
	{"import", BuiltinRPNFunction(
		"import",
		{"path"},
		{ValueType::STRING},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string path = args[0]->getStringValue();
			if (path.size() == 0) {
				return std::make_tuple(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					None::empty()
				);
			}
			return std::make_tuple(
				Module::addModule(path, extractFileName(path), args[0]->getRange(), context->getParent()),
				None::empty()
			);
		}
	)},
	{"importAs", BuiltinRPNFunction(
		"importAs",
		{"path", "name"},
		{ValueType::STRING, ValueType::STRING},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::string path = args[0]->getStringValue();
			if (path.size() == 0) {
				return std::make_tuple(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					None::empty()
				);
			}
			return std::make_tuple(
				Module::addModule(path, args[1]->getStringValue(), args[0]->getRange(), context->getParent()),
				None::empty()
			);
		}
	)}
};
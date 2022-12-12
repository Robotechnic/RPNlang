#include "rpnfunctions/builtinmap.hpp"

const std::unordered_map<std::string, BuiltinRPNFunction> builtins::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{"value"},
		{ValueType::STRING},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			std::cout << args[0]->getStringValue();
			return std::make_pair(ExpressionResult(), None::empty());
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
			return std::make_pair(ExpressionResult(), new String(input, range, Value::INTERPRETER));
		}
	)},
	{"len", BuiltinRPNFunction(
		"len",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_pair(ExpressionResult(), new Int(args[0]->getStringValue().size(), range, Value::INTERPRETER));
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{"value", "start", "length"},
		{ValueType::STRING, ValueType::INT, ValueType::INT},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string value = args[0]->getStringValue();
			int start = static_cast<Int *>(args[1])->getValue();
			int length = static_cast<Int *>(args[2])->getValue();
			return std::make_pair(ExpressionResult(), new String(value.substr(start, length), range, Value::INTERPRETER));
		}
	)},
	{"isNumber", BuiltinRPNFunction(
		"isNumber",
		{"value"},
		{ValueType::STRING},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string value = args[0]->getStringValue();
			bool isNumber = value == "true" ||
							value == "false" ||
							std::regex_match(value.data(), floatRegex) ||
							std::regex_match(value.data(), intRegex);
			return std::make_pair(ExpressionResult(), new Bool(isNumber, range, Value::INTERPRETER));
		}
	)},
	{"toInt", BuiltinRPNFunction(
		"toInt",
		{"value"},
		{ValueType::STRING},
		ValueType::INT,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			RPNFunctionResult result = std::make_pair(ExpressionResult(), nullptr);
			if (std::regex_match(args[0]->getStringValue().data(), intRegex)) {
				result.second = new Int(std::stoi(args[0]->getStringValue().data()), range, Value::INTERPRETER);
			} else {
				result.first = ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to int",
					args[0]->getRange(),
					context
				);
				result.second = Int::empty();
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
			if (std::regex_match(args[0]->getStringValue().data(), floatRegex) || 
				std::regex_match(args[0]->getStringValue().data(), intRegex)
			) {
				result.second = new Float(std::stof(args[0]->getStringValue().data()), range, Value::INTERPRETER);
			} else {
				result.first = ExpressionResult(
					"Cannot convert '" + args[0]->getStringValue() + "' to float",
					args[0]->getRange(),
					context
				);
				result.second = Float::empty();
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
			return std::make_pair(ExpressionResult(), new String(args[0]->getStringValue(), range, Value::INTERPRETER));
		}
	)},
	{"and", BuiltinRPNFunction(
		"and",
		{"value1", "value2"},
		{ValueType::BOOL, ValueType::BOOL},
		ValueType::BOOL,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_pair(
				ExpressionResult(), 
				new Bool(
					static_cast<Bool*>(args[0])->getValue() && static_cast<Bool*>(args[1])->getValue(),
					range,
					Value::INTERPRETER
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
			return std::make_pair(
				ExpressionResult(),
				new Bool(
					static_cast<Bool*>(args[0])->getValue() || static_cast<Bool*>(args[1])->getValue(), 
					range,
					Value::INTERPRETER
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
			return std::make_pair(
				ExpressionResult(), 
				new Bool(
					!static_cast<Bool*>(args[0])->getValue(),
					range,
					Value::INTERPRETER
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
			std::pair<ExpressionResult, Value*> result = std::make_pair(ExpressionResult(), nullptr);
			if (args[0]->getStringValue().size() != 1) {
				result.first = ExpressionResult(
					"string must have length of 1", args[0]->getRange(),
					context
				);
				result.second = Int::empty();
			} else
				result.second = new Int(static_cast<int64_t>(args[0]->getStringValue()[0]), range, Value::INTERPRETER);

			return result;
		}
	)},
	{"chr", BuiltinRPNFunction(
		"chr",
		{"value"},
		{ValueType::INT},
		ValueType::STRING,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			return std::make_pair(
				ExpressionResult(), 
				new String(
					std::string(1, static_cast<char>(static_cast<Int*>(args[0])->getValue())), 
					range,
					Value::INTERPRETER
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
			return std::make_pair(ExpressionResult(), None::empty());
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
					range,
					context->getParent()
				);
			}

			return std::make_pair(result, None::empty());
		}
	)},
	{"import", BuiltinRPNFunction(
		"import",
		{"path"},
		{ValueType::STRING},
		ValueType::NONE,
		[](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
			const std::string path = args[0]->getStringValue();
			if (path.size() == 0) {
				return std::make_pair(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					None::empty()
				);
			}
			return std::make_pair(
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
			const std::string path = args[0]->getStringValue();
			const std::string name = args[1]->getStringValue();
			if (path.size() == 0) {
				return std::make_pair(
					ExpressionResult(
						"import path cannot be empty",
						args[0]->getRange(), 
						context
					), 
					None::empty()
				);
			}
			if (!std::regex_match(name.data(), literalRegex)) {
				return std::make_pair(
					ExpressionResult(
						"Module name must be a valid identifier",
						args[1]->getRange(), 
						context
					), 
					None::empty()
				);
			}
			return std::make_pair(
				Module::addModule(path, name, args[0]->getRange(), context->getParent()),
				None::empty()
			);
		}
	)}
};
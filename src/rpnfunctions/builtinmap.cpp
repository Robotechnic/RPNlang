#include "rpnfunctions/builtinmap.hpp"

const std::unordered_map<std::string, BuiltinRPNFunction> builtins::builtinFunctions = {
	{"print", BuiltinRPNFunction(
		"print",
		{{"value", STRING}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			std::cout << args[0]->getStringValue();
			return None::empty();
		}
	)},
	{"input", BuiltinRPNFunction(
		"input",
		{{"value", STRING}},
		STRING,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			std::cout<<args[0]->getStringValue();
			std::string input;
			std::getline(std::cin, input);
			return new String(input, range, Value::INTERPRETER);
		}
	)},
	{"getChr", BuiltinRPNFunction(
		"getChr",
		{},
		STRING,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			char c = std::getchar();
			return new String(std::string(1, c), range, Value::INTERPRETER);
		}
	)},
	{"len", BuiltinRPNFunction(
		"len",
		{{"value", ANY}},
		INT,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			if (args[0]->getType() == LIST)
				return new Int(
					static_cast<List*>(args[0])->size(),
					range,
					Value::INTERPRETER
				);
			
			return new Int(args[0]->getStringValue().size(), range, Value::INTERPRETER);
		}
	)},
	{"substr", BuiltinRPNFunction(
		"substr",
		{{"value", STRING}, {"start", INT}, {"length", INT}},
		STRING,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			const std::string value = args[0]->getStringValue();
			int start = static_cast<Int *>(args[1])->getValue();
			int length = static_cast<Int *>(args[2])->getValue();
			return new String(value.substr(start, length), range, Value::INTERPRETER);
		}
	)},
	{"isNumber", BuiltinRPNFunction(
		"isNumber",
		{{"value", STRING}},
		BOOL,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			const std::string value = args[0]->getStringValue();
			matchResult result;
			bool isNumber = value == "true" ||
							value == "false" ||
							((result = floatMatch(value)) && result.value().second == value.size()) ||
							((result = intMatch(value)) && result.value().second == value.size());
			return new Bool(isNumber, range, Value::INTERPRETER);
		}
	)},
	{"toInt", BuiltinRPNFunction(
		"toInt",
		{{"value", STRING}},
		INT,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			if (intMatch(args[0]->getStringValue().data())) {
				return new Int(std::stoi(args[0]->getStringValue().data()), range, Value::INTERPRETER);
			}
			return ExpressionResult(
				"Cannot convert '" + args[0]->getStringValue() + "' to int",
				args[0]->getRange(),
				context
			);
		}
	)},
	{"toFloat", BuiltinRPNFunction(
		"toFloat",
		{{"value", STRING}},
		FLOAT,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			if (floatMatch(args[0]->getStringValue().data()) || 
				intMatch(args[0]->getStringValue().data())
			) {
				return new Float(std::stof(args[0]->getStringValue().data()), range, Value::INTERPRETER);
			}
			return ExpressionResult(
				"Cannot convert '" + args[0]->getStringValue() + "' to float",
				args[0]->getRange(),
				context
			);
		}
	)},
	{"toString", BuiltinRPNFunction(
		"toString",
		{{"value", FLOAT}},
		STRING,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			return new String(args[0]->getStringValue(), range, Value::INTERPRETER);
		}
	)},
	{"and", BuiltinRPNFunction(
		"and",
		{{"value1", BOOL}, {"value2", BOOL}},
		BOOL,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			return new Bool(
				static_cast<Bool*>(args[0])->getValue() && static_cast<Bool*>(args[1])->getValue(),
				range,
				Value::INTERPRETER
			);
		}
	)},
	{"or", BuiltinRPNFunction(
		"or",
		{{"value1", BOOL}, {"value2", BOOL}},
		BOOL,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			return new Bool(
				static_cast<Bool*>(args[0])->getValue() || static_cast<Bool*>(args[1])->getValue(), 
				range,
				Value::INTERPRETER
			);
		}
	)},
	{"not", BuiltinRPNFunction(
		"not",
		{{"value", BOOL}},
		BOOL,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			return new Bool(
				!static_cast<Bool*>(args[0])->getValue(),
				range,
				Value::INTERPRETER
			);
		}
	)},
	{"ord", BuiltinRPNFunction(
		"ord",
		{{"value", STRING}},
		INT,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			if (args[0]->getStringValue().size() != 1)
				return ExpressionResult(
					"string must have length of 1", args[0]->getRange(),
					context
				);
			
			return new Int(static_cast<int64_t>(args[0]->getStringValue()[0]), range, Value::INTERPRETER);
		}
	)},
	{"chr", BuiltinRPNFunction(
		"chr",
		{{"value", INT}},
		STRING,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			return new String(
				std::string(1, static_cast<char>(static_cast<Int*>(args[0])->getValue())), 
				range,
				Value::INTERPRETER
			);
		}
	)},
	{"exit", BuiltinRPNFunction(
		"exit",
		{{"value", INT}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			exit(static_cast<Int*>(args[0])->getValue());
			// Should never reach this point
			return None::empty();
		}
	)},
	{"assert",BuiltinRPNFunction(
		"assert",
		{{"value", BOOL}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			ExpressionResult result;

			if (!static_cast<Bool *>(args[0])->getValue()) {
				return ExpressionResult(
					"Assertion failed",
					range,
					context->getParent()
				);
			}

			return None::empty();
		}
	)},
	{"at", BuiltinRPNFunction(
		"at",
		{{"value", ANY}, {"index", INT}},
		ANY,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			Int *index = static_cast<Int *>(args[1]);
			Value *value = nullptr;
			if (args[0]->getType() == LIST) {
				List *list = static_cast<List *>(args[0]);
				if (index->getValue() < 0 || index->getValue() >= list->size()) {
					return ExpressionResult(
						"Index out of range",
						args[1]->getRange(),
						context
					);
				}
				value = list->at(index->getValue());
			} else if (args[0]->getType() == STRING) {
				std::string string = args[0]->getStringValue();
				if (index->getValue() < 0 || (size_t)index->getValue() >= string.size()) {
					return ExpressionResult(
						"Index out of range",
						args[1]->getRange(),
						context
					);
				}
				TextRange charRange = args[0]->getRange();
				charRange.columnStart += index->getValue();
				charRange.columnEnd = charRange.columnStart + 1;
				value = new String(std::string(string[index->getValue()], 1), charRange, Value::INTERPRETER);
			} else {
				return ExpressionResult(
					"Expected list or string as first argument",
					args[0]->getRange(),
					context
				);
			}
			
			value->setVariableRange(TextRange::merge(
				args[0]->getRange(),
				range
			));
			return value;
		}
	)},
	{"set", BuiltinRPNFunction (
		"set",
		{{"list", LIST}, {"index", INT}, {"value", ANY}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			List *list = static_cast<List *>(args[0]);
			Int *index = static_cast<Int *>(args[1]);
			if (index->getValue() < 0 || index->getValue() >= list->size()) {
				return ExpressionResult(
					"Index out of range",
					args[1]->getRange(),
					context
				);
			}
			list->set(index->getValue(), args[2]);
			return None::empty();
		}
	)},
	{"top", BuiltinRPNFunction(
		"top",
		{{"value", LIST}},
		ANY,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			List *list = static_cast<List *>(args[0]);
			if (list->size() == 0) {
				return ExpressionResult(
					"Cannot get top of empty list",
					args[0]->getRange(),
					context
				);
			}
			return list->at(list->size() - 1);
		}
	)},
	{"push", BuiltinRPNFunction(
		"push",
		{{"list", LIST}, {"value", ANY}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			if (args[1]->getOwner() == Value::CONTEXT_VARIABLE || args[1]->getOwner() == Value::OBJECT_VALUE)
				static_cast<List *>(args[0])->push(args[1]->copy(Value::OBJECT_VALUE));
			else
				static_cast<List *>(args[0])->push(args[1]);
			return None::empty();
		}
	)},
	{"pop", BuiltinRPNFunction(
		"pop",
		{{"list", LIST}},
		ANY,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			List *list = static_cast<List *>(args[0]);
			if (list->size() == 0) {
				return ExpressionResult(
					"Cannot pop from empty list",
					args[0]->getRange(),
					context
				);
			}
			Value *result = list->pop();
			return result;
		}
	)},
	{"insert", BuiltinRPNFunction(
		"insert",
		{{"list", LIST}, {"index", INT}, {"value", ANY}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			List *list = static_cast<List *>(args[0]);
			Int *index = static_cast<Int *>(args[1]);
			if (index->getValue() < 0 || index->getValue() > list->size()) {
				return ExpressionResult(
					"Index out of range",
					args[1]->getRange(),
					context
				);
			}
			if (args[2]->getOwner() == Value::CONTEXT_VARIABLE || args[2]->getOwner() == Value::OBJECT_VALUE)
				list->insert(index->getValue(), args[2]->copy(Value::OBJECT_VALUE));
			else
				list->insert(index->getValue(), args[2]);
			return None::empty();
		}
	)},
	{"remove", BuiltinRPNFunction(
		"remove",
		{{"list", LIST}, {"index", INT}},
		ANY,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			List *list = static_cast<List *>(args[0]);
			Int *index = static_cast<Int *>(args[1]);
			if (index->getValue() < 0 || index->getValue() >= list->size()) {
				return ExpressionResult(
					"Index out of range",
					args[1]->getRange(),
					context
				);
			}

			list->remove(index->getValue());
			return None::empty();
		}
	)},
	{"clear", BuiltinRPNFunction(
		"clear",
		{{"list", LIST}},
		NONE,
		[](RPNFunctionArgsValue &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
			static_cast<List *>(args[0])->clear();
			return None::empty();
		}
	)}
};
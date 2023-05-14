#include "math.hpp"

ExpressionResult loader(CppModule *module) {
	module->addFunction("sqrt", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							float value = static_cast<Float *>(args[0])->getValue();
							if (value <= 0) {
								return ExpressionResult(
									"Cannot take square root of negative number", range, context);
							}
							return new Float(std::sqrt(value), range, Value::INTERPRETER);
						});

	module->addFunction("pow", {{"base", FLOAT}, {"exponent", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::pow(static_cast<Float *>(args[0])->getValue(),
													  static_cast<Float *>(args[1])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("sin", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::sin(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("cos", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::cos(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("tan", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::tan(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("asin", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::asin(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("acos", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::acos(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("atan", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::atan(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("atan2", {{"y", FLOAT}, {"x", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::atan2(static_cast<Float *>(args[0])->getValue(),
														static_cast<Float *>(args[1])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("sinh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::sinh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("cosh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::cosh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("tanh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::tanh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("asinh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::asinh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("acosh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::acosh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("atanh", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::atanh(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("exp", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::exp(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("log", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::log(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("log10", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::log10(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("log2", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::log2(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("log", {{"base", FLOAT}, {"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(
								std::log(static_cast<Float *>(args[1])->getValue()) /
									std::log(static_cast<Float *>(args[0])->getValue()),
								range, Value::INTERPRETER);
						});

	module->addFunction("floor", {{"value", FLOAT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::floor(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("ceil", {{"value", FLOAT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::ceil(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("round", {{"value", FLOAT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::round(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("abs", {{"value", INT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Int(std::abs(static_cast<Int *>(args[0])->getValue()), range,
										   Value::INTERPRETER);
						});

	module->addFunction("fabs", {{"value", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::abs(static_cast<Float *>(args[0])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction(
		"deg", {{"value", FLOAT}}, FLOAT,
		[](RPNFunctionArgsValue args, const TextRange &range,
		   ContextPtr context) -> RPNFunctionResult {
			return new Float(
				static_cast<float>(static_cast<Float *>(args[0])->getValue() * 180.0 / M_PI), range,
				Value::INTERPRETER);
		});

	module->addFunction(
		"rad", {{"value", FLOAT}}, FLOAT,
		[](RPNFunctionArgsValue args, const TextRange &range,
		   ContextPtr context) -> RPNFunctionResult {
			return new Float(
				static_cast<float>(static_cast<Float *>(args[0])->getValue() * M_PI / 180.0), range,
				Value::INTERPRETER);
		});

	module->addFunction("min", {{"value1", INT}, {"value2", INT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Int(std::min(static_cast<Int *>(args[0])->getValue(),
													static_cast<Int *>(args[1])->getValue()),
										   range, Value::INTERPRETER);
						});

	module->addFunction("minf", {{"value1", FLOAT}, {"value2", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::min(static_cast<Float *>(args[0])->getValue(),
													  static_cast<Float *>(args[1])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction("max", {{"value1", INT}, {"value2", INT}}, INT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Int(std::max(static_cast<Int *>(args[0])->getValue(),
													static_cast<Int *>(args[1])->getValue()),
										   range, Value::INTERPRETER);
						});

	module->addFunction("maxf", {{"value1", FLOAT}, {"value2", FLOAT}}, FLOAT,
						[](RPNFunctionArgsValue args, const TextRange &range,
						   ContextPtr context) -> RPNFunctionResult {
							return new Float(std::max(static_cast<Float *>(args[0])->getValue(),
													  static_cast<Float *>(args[1])->getValue()),
											 range, Value::INTERPRETER);
						});

	module->addFunction(
		"map",
		{{"value", FLOAT}, {"min", FLOAT}, {"max", FLOAT}, {"newMin", FLOAT}, {"newMax", FLOAT}},
		FLOAT,
		[](RPNFunctionArgsValue args, const TextRange &range,
		   ContextPtr context) -> RPNFunctionResult {
			// simple linear mapping
			// (value - min) * (newMax - newMin) / (max - min) + newMin
			return new Float((static_cast<Float *>(args[0])->getValue() -
							  static_cast<Float *>(args[1])->getValue()) *
									 (static_cast<Float *>(args[4])->getValue() -
									  static_cast<Float *>(args[3])->getValue()) /
									 (static_cast<Float *>(args[2])->getValue() -
									  static_cast<Float *>(args[1])->getValue()) +
								 static_cast<Float *>(args[3])->getValue(),
							 range, Value::INTERPRETER);
		});

	module->addVariable("PI", new Float((float)M_PI, TextRange(), Value::CONTEXT_VARIABLE));

	module->addVariable("E", new Float((float)M_E, TextRange(), Value::CONTEXT_VARIABLE));

	module->addVariable("PHI", new Float(1.618033988749f, TextRange(), Value::CONTEXT_VARIABLE));

	module->addVariable("TAU", new Float((float)M_PI_2, TextRange(), Value::CONTEXT_VARIABLE));

	return ExpressionResult();
}
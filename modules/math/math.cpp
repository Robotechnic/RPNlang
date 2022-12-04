#include "math.hpp"

ExpressionResult loader(CppModule &module){
	module.addFunction("sqrt", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Float(
				std::sqrt(static_cast<Float *>(args[0])->getValue()), 
				range,
				true
			)
		);
	});

	module.addFunction("pow", {"base", "exponent"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::pow(static_cast<Float *>(args[0])->getValue(), static_cast<Float *>(args[1])->getValue()), range, true
			)
		);
	});

	module.addFunction("sin", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::sin(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("cos", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::cos(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("tan", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::tan(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("asin", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::asin(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("acos", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::acos(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("atan", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::atan(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("atan2", {"y", "x"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::atan2(static_cast<Float *>(args[0])->getValue(), static_cast<Float *>(args[1])->getValue()), range, true
			)
		);
	});

	module.addFunction("sinh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::sinh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("cosh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::cosh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("tanh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::tanh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("asinh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::asinh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("acosh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::acosh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("atanh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::atanh(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("exp", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::exp(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("log", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::log(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("log10", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::log10(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("log2", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::log2(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("log", {"base", "value"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::log(static_cast<Float *>(args[1])->getValue()) / 
				std::log(static_cast<Float *>(args[0])->getValue()), 
				range,
				true
			)
		);
	});

	module.addFunction("floor", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::floor(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("ceil", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::ceil(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("round", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::round(static_cast<Float *>(args[0])->getValue()), range, true
			)
		);
	});

	module.addFunction("abs", {"value"}, {INT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Int(
				std::abs(static_cast<Int *>(args[0])->getValue()), 
				range,
				true
			)
		);
	});

	module.addFunction("fabs", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				std::abs(static_cast<Float *>(args[0])->getValue()), 
				range,
				true
			)
		);
	});

	module.addFunction("deg", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				static_cast<float>(static_cast<Float *>(args[0])->getValue() * 180.0 / M_PI), range, true
			)
		);
	});

	module.addFunction("rad", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				static_cast<float>(static_cast<Float *>(args[0])->getValue() * M_PI / 180.0), range, true
			)
		);
	});

	module.addFunction("map", {"value", "min", "max", "newMin", "newMax"}, {FLOAT, FLOAT, FLOAT, FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		// simple linear mapping
		// (value - min) * (newMax - newMin) / (max - min) + newMin
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				(static_cast<Float *>(args[0])->getValue() - static_cast<Float *>(args[1])->getValue()) *
				(static_cast<Float *>(args[4])->getValue() - static_cast<Float *>(args[3])->getValue()) /
				(static_cast<Float *>(args[2])->getValue() - static_cast<Float *>(args[1])->getValue()) +
				static_cast<Float *>(args[3])->getValue(),
				range,
				true
			)
		);
	});

	module.addVariable("PI",
		new Float(
			(float)M_PI,
			TextRange(),
			false
		)
	);

	module.addVariable("E",
		new Float(
			(float)M_E,
			TextRange(),
			false
		)
	);

	module.addVariable("PHI",
		new Float(
			1.618033988749f,
			TextRange(),
			false
		)
	);

	module.addVariable("TAU",
		new Float(
			(float)M_PI_2,
			TextRange(),
			false
		)
	);

	return ExpressionResult();
}
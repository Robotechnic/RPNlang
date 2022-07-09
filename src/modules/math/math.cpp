#include "modules/math/math.hpp"

ExpressionResult mathLoader(BuiltinModule &module){
	module.addFunction("sqrt", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::sqrt(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("pow", {"base", "exponent"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::pow(args[0].getFloatValue(), args[1].getFloatValue()), TextRange()));
	});

	module.addFunction("sin", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::sin(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("cos", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::cos(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("tan", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::tan(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("asin", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::asin(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("acos", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::acos(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("atan", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::atan(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("atan2", {"y", "x"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::atan2(args[0].getFloatValue(), args[1].getFloatValue()), TextRange()));
	});

	module.addFunction("sinh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::sinh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("cosh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::cosh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("tanh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::tanh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("asinh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::asinh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("acosh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::acosh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("atanh", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::atanh(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("exp", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::exp(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("log", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::log(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("log10", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::log10(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("log2", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::log2(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("log", {"base", "value"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::log(args[1].getFloatValue()) / std::log(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("floor", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::floor(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("ceil", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::ceil(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("round", {"value"}, {FLOAT}, INT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(std::round(args[0].getFloatValue()), TextRange()));
	});

	module.addFunction("abs", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		if (args[0].getType() == FLOAT)
			return std::make_tuple(ExpressionResult(), Value(std::abs(args[0].getFloatValue()), TextRange()));
		else
			return std::make_tuple(ExpressionResult(), Value(std::abs(args[0].getIntValue()), TextRange()));
	});

	module.addFunction("deg", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(static_cast<float>(args[0].getFloatValue() * 180.0 / M_PI), TextRange()));
	});

	module.addFunction("rad", {"value"}, {FLOAT}, FLOAT, [](RPNFunctionArgs args, Context* context) {
		return std::make_tuple(ExpressionResult(), Value(static_cast<float>(args[0].getFloatValue() * M_PI / 180.0), TextRange()));
	});

	module.addVariable("PI", Value(
		(float)M_PI,
		TextRange()
	));

	module.addVariable("E", Value(
		(float)M_E,
		TextRange()
	));

	module.addVariable("PHI", Value(
		1.618033988749f,
		TextRange()
	));

	module.addVariable("TAU", Value(
		(float)M_PI_2,
		TextRange()
	));

	return ExpressionResult();
}
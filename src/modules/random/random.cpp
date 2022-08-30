#include "modules/random/random.hpp"


ExpressionResult randomLoader(BuiltinModule &module) {
	module.addFunction("seed", {"seed"}, {INT}, NONE, [](RPNFunctionArgs args, Context *context) {
		std::srand(static_cast<Int *>(args[0])->getValue());
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module.addFunction("rseed", {}, {}, NONE, [](RPNFunctionArgs args, Context *context) {
		std::srand(std::time(nullptr));
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module.addFunction("rand", {}, {}, INT, [](RPNFunctionArgs args, Context *context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Int(
				std::rand(), 
				TextRange()
			)
		);
	});

	module.addFunction("randRange", {"min", "max"}, {INT, INT}, INT, [](RPNFunctionArgs args, Context *context) {
		Int *min = static_cast<Int *>(args[0]);
		Int *max = static_cast<Int *>(args[1]);
		return std::make_tuple(
			ExpressionResult(),
			new Int(
				std::rand() % (max->getValue() - min->getValue() + 1) + min->getValue(), 
				TextRange()
			)
		);
	});

	module.addFunction("randFloat", {}, {}, FLOAT, [](RPNFunctionArgs args, Context *context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Float(
				static_cast <float>(std::rand()) / RAND_MAX, 
				TextRange()
			)
		);
	});

	module.addFunction("randFloatRange", {"min", "max"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, Context *context) {
		Float *min = static_cast<Float *>(args[0]);
		Float *max = static_cast<Float *>(args[1]);
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				min->getValue() + std::rand() / (static_cast <float> (RAND_MAX / (max->getValue() - min->getValue()))),
				TextRange()
			)
		);
	});


	return ExpressionResult();
}
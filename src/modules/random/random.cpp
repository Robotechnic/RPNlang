#include "modules/random/random.hpp"


ExpressionResult randomLoader(CppModule &module) {
	module.addFunction("seed", {"seed"}, {INT}, NONE, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		std::srand(static_cast<Int *>(args[0])->getValue());
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module.addFunction("rseed", {}, {}, NONE, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		std::srand(std::time(nullptr));
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module.addFunction("rand", {}, {}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Int(
				std::rand(), 
				range,
				true
			)
		);
	});

	module.addFunction("randRange", {"min", "max"}, {INT, INT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		Int *min = static_cast<Int *>(args[0]);
		Int *max = static_cast<Int *>(args[1]);
		return std::make_tuple(
			ExpressionResult(),
			new Int(
				std::rand() % (max->getValue() - min->getValue() + 1) + min->getValue(), 
				range,
				true
			)
		);
	});

	module.addFunction("randFloat", {}, {}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		return std::make_tuple(
			ExpressionResult(), 
			new Float(
				static_cast <float>(std::rand()) / RAND_MAX, 
				range,
				true
			)
		);
	});

	module.addFunction("randFloatRange", {"min", "max"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		Float *min = static_cast<Float *>(args[0]);
		Float *max = static_cast<Float *>(args[1]);
		return std::make_tuple(
			ExpressionResult(),
			new Float(
				min->getValue() + std::rand() / (static_cast <float> (RAND_MAX / (max->getValue() - min->getValue()))),
				range,
				true
			)
		);
	});


	return ExpressionResult();
}
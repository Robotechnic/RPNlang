#include "random.hpp"


ExpressionResult loader(CppModule *module) {
	module->addFunction("seed", {"seed"}, {INT}, NONE, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::srand(static_cast<Int *>(args[0])->getValue());
		return None::empty();
	});

	module->addFunction("rseed", {}, {}, NONE, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::srand(std::time(nullptr));
		return None::empty();
	});

	module->addFunction("rand", {}, {}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		return new Int(
			std::rand(), 
			range,
			Value::INTERPRETER
		);
	});

	module->addFunction("randRange", {"min", "max"}, {INT, INT}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Int *min = static_cast<Int *>(args[0]);
		Int *max = static_cast<Int *>(args[1]);
		return new Int(
			std::rand() % (max->getValue() - min->getValue() + 1) + min->getValue(), 
			range,
			Value::INTERPRETER
		);
	});

	module->addFunction("randFloat", {}, {}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		return new Float(
			static_cast <float>(std::rand()) / RAND_MAX, 
			range,
			Value::INTERPRETER
		);
	});

	module->addFunction("randFloatRange", {"min", "max"}, {FLOAT, FLOAT}, FLOAT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		Float *min = static_cast<Float *>(args[0]);
		Float *max = static_cast<Float *>(args[1]);
		return new Float(
			min->getValue() + std::rand() / (static_cast <float> (RAND_MAX / (max->getValue() - min->getValue()))),
			range,
			Value::INTERPRETER
		);
	});


	return ExpressionResult();
}
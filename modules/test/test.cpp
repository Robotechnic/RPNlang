#include "test.hpp"

ExpressionResult loader(CppModule *module) {
	module->addFunction("testFunction", {"value"}, {STRING}, NONE, [](RPNFunctionArgs &args, const TextRange &range, ContextPtr context) -> RPNFunctionResult {
		std::cout<<"Test ok : "<<args[0]->getStringValue()<<std::endl;
		return None::empty();
	});

	module->addVariable("testValue", new String(
		"testValueOk",
		TextRange(),
		Value::CONTEXT_VARIABLE
	));
	return ExpressionResult();
};


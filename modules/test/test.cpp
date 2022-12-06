#include "test.hpp"

ExpressionResult loader(CppModule *module) {
	module->addFunction("testFunction", {"value"}, {STRING}, NONE, [](const RPNFunctionArgs &args, const TextRange &range, ContextPtr context) {
		std::cout<<"Test ok : "<<args[0]->getStringValue()<<std::endl;
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module->addVariable("testValue", new String(
		std::string("testValueOk"),
		TextRange(),
		false
	));
	return ExpressionResult();
};

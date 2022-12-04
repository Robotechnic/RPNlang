#include "modules/time/time.hpp"

ExpressionResult timeLoader(CppModule &module) {
	module.addFunction("sleep", {"time"}, {FLOAT}, NONE, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<Int *>(args[0])->getValue()));
		return std::make_tuple(ExpressionResult(), None::empty());
	});

	module.addFunction("time", {}, {}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		using namespace std::chrono;
		return std::make_tuple(
			ExpressionResult(), 
			new Int(
				duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(), 
				range,
				true
			)
		);
	});

	module.addFunction("timeMicros", {}, {}, INT, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		using namespace std::chrono;
		return std::make_tuple(
			ExpressionResult(), 
			new Int(
				duration_cast<microseconds>(system_clock::now().time_since_epoch()).count(), 
				range,
				true
			)
		);
	});

	module.addFunction("strTime", {"format"}, {STRING}, STRING, [](RPNFunctionArgs args, const TextRange &range, ContextPtr context) {
		std::time_t t = std::time(nullptr);
		char buffer[100];
		if (std::strftime(buffer, sizeof(buffer), args[0]->getStringValue().c_str(), std::localtime(&t)))
			return std::make_tuple(
				ExpressionResult(), 
				new String(
					std::string(buffer), 
					range,
					true
				)
			);

		return std::make_tuple(
				ExpressionResult(
					"Could not parse date format: " + args[0]->getStringValue(), 
					args[0]->getRange(),
					context
				), 
				String::empty()
			);
	});
		
	return ExpressionResult();
};
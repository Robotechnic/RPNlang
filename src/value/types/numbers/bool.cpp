#include "value/types/numbers/bool.hpp"

Bool::Bool(std::string value, TextRange range) : 
	Value(BOOL, range),
	value(value != "false"){}

Bool::Bool(bool value, TextRange range) : 
	Value(BOOL, range),
	value(value){}

bool Bool::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == INT ||
		type == FLOAT ||
		type == BOOL;
}

Value *Bool::to(ValueType type) {
	switch (type) {
		case STRING:
			return new String(value ? "true" : "false", range);
		case INT:
			return new Int(static_cast<int64_t>(value), range);
		case FLOAT:
			return new Float(static_cast<float>(value), range);
		case BOOL:
			return this;
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Bool::copy() const {
	return new Bool(value, range);
}

std::string Bool::getStringValue() const {
	return  value ? "true" : "false";
}

operatorReturn Bool::opadd(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot add value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value + static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value + static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value + static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opsub(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot subtract value of type " + Value::stringType(other->getType()) + " from Int",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value - static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value - static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value - static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opmul(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot multiply INT by value of type " + Value::stringType(other->getType()),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value * static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Int(value * static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value * static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opdiv(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot divide INT number by value of type " + Value::stringType(other->getType()),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT: {
			int64_t otherValue = static_cast<Int const*>(other)->getValue();
			if (otherValue == 0)
				return std::make_tuple(
					ExpressionResult(
						"Division by 0",
						other->getRange(),
						context
					),
					&Int::emptyInt
				);
			return std::make_tuple(
				ExpressionResult(),
				new Int(value / otherValue, range)
			);
		}
		case BOOL: {
			bool otherValue = static_cast<Bool const*>(other)->getValue();
			if (!otherValue)
				return std::make_tuple(
					ExpressionResult(
						"Division by false",
						other->getRange(),
						context
					),
					&Int::emptyInt
				);
			return std::make_tuple(
				ExpressionResult(),
				new Int(value / otherValue, range)
			);
		}
		case FLOAT: {
			float otherValue = static_cast<Float const*>(other)->getValue();
			if (otherValue == 0.0)
				return std::make_tuple(
					ExpressionResult(
						"Division by 0.0",
						other->getRange(),
						context
					),
					&Float::emptyFloat
				);
			return std::make_tuple(
				ExpressionResult(),
				new Float(value / otherValue, range)
			);
		}
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opmod(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot modulo INT by value of type " + Value::stringType(other->getType()),
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT: {
			int64_t otherValue = static_cast<Int const*>(other)->getValue();
			if (otherValue == 0)
				return std::make_tuple(
					ExpressionResult(
						"Modulo by 0",
						other->getRange(),
						context
					),
					nullptr
				);
			return std::make_tuple(
				ExpressionResult(),
				new Int(value % otherValue, range)
			);
		}
		case BOOL: {
			bool otherValue = static_cast<Bool const*>(other)->getValue();
			if (!otherValue)
				return std::make_tuple(
					ExpressionResult(
						"Modulo by false",
						other->getRange(),
						context
					),
					nullptr
				);
			return std::make_tuple(
				ExpressionResult(),
				new Int(value % otherValue, range)
			);
		}
		case FLOAT: {
			float otherValue = static_cast<Float const*>(other)->getValue();
			if (otherValue == 0.0)
				return std::make_tuple(
					ExpressionResult(
						"Modulo by 0.0",
						other->getRange(),
						context
					),
					&Float::emptyFloat
				);
			return std::make_tuple(
				ExpressionResult(),
				new Float(std::fmod(value, otherValue), range)
			);
		}
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::oppow(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot raise value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Int(std::pow(value, static_cast<Int const*>(other)->getValue()), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Int(std::pow(value, static_cast<Bool const*>(other)->getValue()), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Float const*>(other)->getValue()), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opgt(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value > static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value > static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value > static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opge(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value >= static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value >= static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value >= static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::oplt(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value < static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value < static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value < static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::ople(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Bool",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value <= static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value <= static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value <= static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opne(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(),
			new Bool(true, range)
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value != static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value != static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value != static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

operatorReturn Bool::opeq(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(),
			new Bool(false, range)
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value == static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value == static_cast<Bool const*>(other)->getValue(), range)
			);
		case FLOAT:
			return std::make_tuple(
				ExpressionResult(),
				new Bool(value == static_cast<Float const*>(other)->getValue(), range)
			);
		default:
			return std::make_tuple(ExpressionResult(),	nullptr);
	};
}

Bool Bool::emptyBool = Bool(false, TextRange());
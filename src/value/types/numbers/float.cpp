#include "value/types/numbers/float.hpp"

Float::Float(std::string value, TextRange range) : 
	Value(FLOAT, range),
	value(std::stof(value)){}

Float::Float(float value, TextRange range) : 
	Value(FLOAT, range),
	value(value) {}

bool Float::isCastableTo(ValueType type) const {
	return 
		type == STRING ||
		type == INT ||
		type == FLOAT ||
		type == BOOL;
}

Value *Float::to(ValueType type) {
	switch (type) {
		case STRING:
			return new String(std::to_string(value), range);
		case INT:
			return new Int(static_cast<int64_t>(value), range);
		case FLOAT:
			return this;
		case BOOL:
			return new Bool(value != 0, range);
		default:
			throw std::runtime_error("Invalid value type");
	};
}

Value *Float::copy() const {
	return new Float(value, range);
}

std::string Float::getStringValue() const {
	return  std::to_string(value);
}

operatorResult Float::opadd(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot add value of type " + Value::stringType(other->getType()) + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value + static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value + static_cast<Bool const*>(other)->getValue(), range)
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

operatorResult Float::opsub(const Value *other, const Context *context) {
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
				new Float(value - static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value - static_cast<Bool const*>(other)->getValue(), range)
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

operatorResult Float::opmul(const Value *other, const Context *context) {
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
				new Float(value * static_cast<Int const*>(other)->getValue(), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Float(value * static_cast<Bool const*>(other)->getValue(), range)
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

operatorResult Float::opdiv(const Value *other, const Context *context) {
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
				new Float(value / otherValue, range)
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
				new Float(value / otherValue, range)
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

operatorResult Float::opmod(const Value *other, const Context *context) {
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
				new Float(std::fmod(value, otherValue), range)
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
				new Float(std::fmod(value, otherValue), range)
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

operatorResult Float::oppow(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot raise value of type " + Value::stringType(other->getType()) + " to Float",
				other->getRange(),
				context
			),
			nullptr
		);
	
	switch (other->getType()) {
		case INT:
			return std::make_tuple(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Int const*>(other)->getValue()), range)
			);
		case BOOL:
			return std::make_tuple(
				ExpressionResult(),
				new Float(std::pow(value, static_cast<Bool const*>(other)->getValue()), range)
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

operatorResult Float::opgt(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Float",
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

operatorResult Float::opge(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Float",
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

operatorResult Float::oplt(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Float",
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

operatorResult Float::ople(const Value *other, const Context *context) {
	if (!other->isNumber())
		return std::make_tuple(
			ExpressionResult(
				"Cannot compare value of type " + Value::stringType(other->getType()) + " to Float",
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

operatorResult Float::opne(const Value *other, const Context *context) {
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

operatorResult Float::opeq(const Value *other, const Context *context) {
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

Float Float::emptyFloat = Float(0, TextRange());
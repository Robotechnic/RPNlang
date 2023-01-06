#include "value/types/list.hpp"

List::List(TextRange range, ValueOwner owner) : Value(ValueType::LIST, range, owner) {}
List::List(std::vector<Value *> values, TextRange range, ValueOwner owner) : 
	Value(ValueType::LIST, range, owner), 
	values(values)
{}

List::~List() {
	for (auto &value : values) {
		Value::deleteValue(&value, Value::OBJECT_VALUE);
	}
}

bool List::isCastableTo(ValueType type) const {
	return type == ValueType::LIST || type == ValueType::STRING || type == ValueType::ANY;
}

bool List::isNumber() const {
	return false;
}

std::vector<Value *> List::getValues() const {
	return values;
}

unsigned int List::size() const {
	return values.size();
}

Value *List::at(unsigned int index) const {
	if (index >= values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	return values[index];
}

void List::set(unsigned int index, Value *value) {
	if (index >= values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	Value::deleteValue(&values[index], Value::OBJECT_VALUE);
	value->setOwner(Value::OBJECT_VALUE);
	this->values[index] = value;
}

void List::push(Value *value) {
	value->setOwner(Value::OBJECT_VALUE);
	this->values.push_back(value);
}

Value * List::pop() {
	if (values.size() == 0) {
		throw std::runtime_error("Cannot pop from empty list");
	}
	Value *value = values.back();
	values.pop_back();
	return value;
}

void List::insert(unsigned int index, Value *value) {
	if (index > values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	value->setOwner(Value::OBJECT_VALUE);
	this->values.insert(values.begin() + index, value);
}
void List::remove(unsigned int index) {
	if (index >= values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	Value::deleteValue(&values[index], Value::OBJECT_VALUE);
	values.erase(values.begin() + index);
}

void List::clear() {
	for (auto &value : values) {
		Value::deleteValue(&value, Value::OBJECT_VALUE);
	}
	values.clear();
}

Value *List::to(ValueType type, ValueOwner owner) const {
	if (type == ValueType::LIST || type == ValueType::ANY) {
		return copy(owner);
	} else if (type == ValueType::STRING) {
		return new String(this->getStringValue(), range, owner);
	} else {
		throw std::runtime_error("Cannot cast list to " + std::string(Value::stringType(type)));
	}
}
inline Value *List::copy(ValueOwner owner) const {
	std::vector<Value *> newValues;
	for (auto &value : values) {
		newValues.push_back(value->copy(Value::OBJECT_VALUE));
	}
	return new List(newValues, range, owner);
}

inline std::string List::getStringValue() const {
	std::string result = "[";
	for (auto &value : values) {
		result += value->getStringValue() + ", ";
	}
	if (result.size() > 1) {
		result.pop_back();
		result.pop_back();
	}
	result += "]";
	return result;
}

operatorResult List::opadd(const Value *other, const ContextPtr &context) const {
	if (other->getType() == ValueType::LIST) {
		std::vector<Value *> newValues = values;
		for (auto &value : static_cast<const List*>(other)->values) {
			newValues.push_back(value->copy(Value::OBJECT_VALUE));
		}
		return std::make_pair<ExpressionResult, Value*>(
			ExpressionResult(),
			new List(newValues, range, INTERPRETER)
		);
	}
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot add element of type " + Value::stringType(other->getType()) + " to list",
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opsub(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot subtract element of type " + Value::stringType(other->getType()) + " from list",
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opmul(const Value *other, const ContextPtr &context) const {
	if (other->getType() == ValueType::INT) {
		std::vector<Value *> newValues;
		int valueCount = static_cast<const Int*>(other)->getValue();
		if (valueCount < 0) {
			return std::make_pair<ExpressionResult, Value*>(
				ExpressionResult(
					"Cannot multiply list by negative integer",
					range,
					context
				),
				nullptr
			);
		}
		for (int i = 0; i < valueCount; i++) {
			for (auto &value : values) {
				newValues.push_back(value->copy(Value::OBJECT_VALUE));
			}
		}
		return std::make_pair<ExpressionResult, Value*>(
			ExpressionResult(),
			new List(newValues, range, INTERPRETER)
		);
	}
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot multiply list by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opdiv(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot divide list by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opmod(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot mod list by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::oppow(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot raise list to the power of element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}


operatorResult List::opgt(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare list to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opge(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare list to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::oplt(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare list to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::ople(const Value *other, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare list to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult List::opne(const Value *other, const ContextPtr &context) const {
	bool result = true;
	if (other->getType() == ValueType::LIST) {
		const std::vector<Value*> otherList = static_cast<const List*>(other)->values;
		if (values.size() == otherList.size()) {
			result = true;
			for (size_t i = 0; i < values.size() && result; i++) {
				operatorResult opResult = values[i]->opne(otherList[i], context);
				if (opResult.first.error()) {
					return opResult;
				}
				result = static_cast<const Bool*>(opResult.second)->getValue();
			}
			result = !result;
		}
		return std::make_pair<ExpressionResult, Value*>(
			ExpressionResult(),
			new Bool(result, range, INTERPRETER)
		);
	}
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(),
		new Bool(result, range, INTERPRETER)
	);
}

operatorResult List::opeq(const Value *other, const ContextPtr &context) const {
	bool result = false;
	if (other->getType() == ValueType::LIST) {
		const std::vector<Value*> otherList = static_cast<const List*>(other)->values;
		if (values.size() == otherList.size()) {
			result = true;
			for (size_t i = 0; i < values.size() && result; i++) {
				operatorResult opResult = values[i]->opeq(otherList[i], context);
				if (opResult.first.error()) {
					return opResult;
				}
				result = static_cast<const Bool*>(opResult.second)->getValue();
			}
		}
		return std::make_pair<ExpressionResult, Value*>(
			ExpressionResult(),
			new Bool(result, range, INTERPRETER)
		);
	}
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(),
		new Bool(result, range, INTERPRETER)
	);
}

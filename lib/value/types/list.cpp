#include "value/types/list.hpp"

List::List(TextRange range, ValueOwner owner, RPNBaseType listType, const TextRange variableRange) : 
	Value(ValueType::LIST, range, owner, variableRange), values(), listType(listType) {}
List::List(std::vector<Value *> values, TextRange range, ValueOwner owner, RPNBaseType listType, const TextRange variableRange) : 
	Value(ValueType::LIST, range, owner, variableRange), 
	values(values),
	listType(listType)
{}

List::~List() {
	for (auto &value : values) {
		Value::deleteValue(&value, Value::OBJECT_VALUE);
	}
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

Value *&List::at(unsigned int index) {
	if (index >= values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	return values[index];
}

void List::set(unsigned int index, Value *value, Value **hold) {
	if (index >= values.size() || index < 0) {
		throw std::runtime_error("Index out of bounds");
	}
	if (hold != nullptr) {
		*hold = values[index];
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
	if (type == LIST || type == ANY) {
		return copy(owner);
	} else if (type == STRING) {
		return new String(this->getStringValue(), range, owner);
	} else if (type == BOOL) {
		return new Bool(this->size() > 0, range, owner);
	} else {
		throw std::runtime_error("Cannot cast list to " + std::string(stringType(type)));
	}
}
inline Value *List::copy(ValueOwner owner) const {
	std::vector<Value *> newValues;
	for (auto &value : values) {
		newValues.push_back(value->copy(Value::OBJECT_VALUE));
	}
	return new List(newValues, range, owner, this->listType, this->variableRange);
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

Value *List::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() == ValueType::LIST) {
		std::vector<Value *> newValues = values;
		for (auto &value : static_cast<const List*>(other)->values) {
			newValues.push_back(value->copy(Value::OBJECT_VALUE));
		}
		return new List(
			newValues, 
			range,
			INTERPRETER,
			RPNValueType::lessRestrictive(std::get<ValueType>(this->listType), std::get<ValueType>(static_cast<const List*>(other)->listType))
		);
	}
	throw std::runtime_error("Cannot add element of type " + stringType(other->getType()) + " to list");
}

Value *List::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot subtract element of type " + stringType(other->getType()) + " from list");
}

Value *List::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() == ValueType::INT) {
		std::vector<Value *> newValues;
		int valueCount = static_cast<const Int*>(other)->getValue();
		for (int i = 0; i < valueCount; i++) {
			for (auto &value : values) {
				newValues.push_back(value->copy(Value::OBJECT_VALUE));
			}
		}
		return new List(newValues, range, INTERPRETER, this->listType);
	}
	throw std::runtime_error("Cannot multiply list by element of type " + stringType(other->getType()));
}

Value *List::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot divide list by element of type " + stringType(other->getType()));
}

Value *List::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot mod list by element of type " + stringType(other->getType()));
}

Value *List::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot raise list to the power of element of type " + stringType(other->getType()));
}


Value *List::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare list to element of type " + stringType(other->getType()));
}

Value *List::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare list to element of type " + stringType(other->getType()));
}

Value *List::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare list to element of type " + stringType(other->getType()));
}

Value *List::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare list to element of type " + stringType(other->getType()));
}

Value *List::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	bool result = true;
	if (other->getType() == ValueType::LIST) {
		const std::vector<Value*> otherList = static_cast<const List*>(other)->values;
		if (values.size() == otherList.size()) {
			result = true;
			for (size_t i = 0; i < values.size() && result; i++) {
				Value *opResult = values[i]->opne(otherList[i], range, context);
				result = static_cast<const Bool*>(opResult)->getValue();
			}
			result = !result;
		}
		return new Bool(result, range, INTERPRETER);
	}
	return new Bool(result, range, INTERPRETER);
}

Value *List::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	bool result = false;
	if (other->getType() == ValueType::LIST) {
		const std::vector<Value*> otherList = static_cast<const List*>(other)->values;
		if (values.size() == otherList.size()) {
			result = true;
			for (size_t i = 0; i < values.size() && result; i++) {
				Value *opResult = values[i]->opeq(otherList[i], range, context);
				result = static_cast<const Bool*>(opResult)->getValue();
			}
		}
		return new Bool(result, range, INTERPRETER);
	}
	return new Bool(result, range, INTERPRETER);
}

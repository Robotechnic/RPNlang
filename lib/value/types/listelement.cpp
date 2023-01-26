#include "value/types/listelement.hpp"

ListElement::ListElement(List *list, size_t index, TextRange range, ValueOwner owner) : 
	Value(ValueType::LIST_ELEMENT, range, owner), list(list), index(index) {
		if (index >= list->size()) {
			throw std::runtime_error("Index out of bounds");
		}
	}

Value *&ListElement::get() const {
	return list->at(index);
}

void ListElement::set(Value * const &value, Value **hold) {
	this->list->set(index, value, hold);
}

std::string ListElement::getStringValue() const {
	return list->at(index)->getStringValue();
}

Value *ListElement::to(ValueType type, ValueOwner owner) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}

Value *ListElement::copy(ValueOwner owner) const {
	return new ListElement(list, index, range, owner);
}

Value *ListElement::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
Value *ListElement::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("ListElement is just a placeholder and can't be used in ant operations");
}
#include "value/types/struct.hpp"

StructDefinition::StructDefinition(std::string_view name) : name(name) {}

StructDefinition::StructDefinition(const StructDefinition &other) :
	name(other.name),
	members(other.members),
	memberOrder(other.memberOrder) {}

StructDefinition::StructDefinition(StructDefinition &&other) : 
	name(std::move(other.name)),
	members(std::move(other.members)),
	memberOrder(std::move(other.memberOrder)) {}


void StructDefinition::operator=(StructDefinition &other) {
	this->name = other.name;
	this->members = other.members;
	this->memberOrder = other.memberOrder;
}

void StructDefinition::addMember(std::string_view name, RPNValueType type) {
	std::string nameStr = std::string(name);
	this->members[nameStr] = type;
	this->memberOrder.push_back(nameStr.data());
}

bool StructDefinition::hasMember(std::string_view name, RPNValueType *memberType) const {
	auto it = this->members.find(std::string(name));
	if (it == this->members.end()) {
		return false;
	}
	if (memberType != nullptr) {
		*memberType = it->second;
	}
	return true;
}

std::string StructDefinition::getName() const {
	return this->name;
}

size_t StructDefinition::getMembersCount() const {
	return this->members.size();
}

std::vector<std::string> StructDefinition::getMembersOrder() const {
	return this->memberOrder;
}

RPNValueType StructDefinition::getMemberType(std::string_view name) const {
	return this->members.at(std::string(name));
}

void StructDefinition::display(std::ostream &stream) const {
	for (const std::string &key : this->memberOrder) {
		stream << "\t" << key << " -> ";
		if (this->members.at(key).index() == 0) {
			stream << std::get<std::string>(this->members.at(key));
		} else {
			stream << std::get<ValueType>(this->members.at(key));
		}
		stream << std::endl;
	}
}

std::ostream &operator<<(std::ostream &stream, const StructDefinition &definition) {
	stream << "Struct " << definition.getName() << std::endl;
	definition.display(stream);
	stream << "End of Struct " << definition.getName() << std::endl;
	return stream;
}

Struct::Struct(TextRange range, std::string_view name, ValueOwner owner, bool immutable) :
	Value(STRUCT, range, owner), 
	immutable(immutable),
	definition(&definitions[std::string(name)]),
	members(std::make_shared<std::unordered_map<std::string, Value*>>()) {}

Struct::~Struct() {
	if (this->members.use_count() != 1) return;
	for (auto &pair : *this->members) {
		Value::deleteValue(&pair.second, OBJECT_VALUE);
	}
}

/**
 * @brief set all the member at once in order
 * 
 * @param members the values to put in the Struct members
 * @param context the parent context
 * @return ExpressionResult if all the values are correct
 */
ExpressionResult Struct::setMembers(std::vector<Value*> members, ContextPtr context) {
	if (members.size() != this->definition->members.size()) {
		throw std::runtime_error("Member count mismatch, expected " + std::to_string(this->definition->members.size()) + " but " + std::to_string(members.size()) + " were given");
	}
	auto it = members.begin();
	for (const std::string &key : this->definition->memberOrder) {
		RPNValueType type = this->definition->members.at(key);
		if (type.index() == 0) {
			if ((*it)->getType() != STRUCT) {
				return ExpressionResult(
					"Struct " + this->definition->name + " member " + key + 
					" is of type " + std::get<std::string>(type) + " but " + (*it)->getStringType() + " was given",
					(*it)->getRange(),
					context
				);
			}
			Struct *structValue = static_cast<Struct*>(*it);
			if (structValue->definition->name != std::get<std::string>(type)) {
				return ExpressionResult(
					"Struct " + this->definition->name + " member " + key + 
					" is of type " + std::get<std::string>(type) + " but " + std::string(structValue->getStructName()) + " was given",
					(*it)->getRange(),
					context
				);
			}
			(*this->members)[key] = (*it)->copy(OBJECT_VALUE);
		} else {
			if (!Value::isCastableTo((*it)->getType(), std::get<ValueType>(type))) {
				return ExpressionResult(
					"Struct " + this->definition->name + " member " + key + 
					" is of type " + Value::stringType(std::get<ValueType>(type)) + " but " + (*it)->getStringType() + " was given",
					(*it)->getRange(),
					context
				);
			}
			(*this->members)[key] = (*it)->to(std::get<ValueType>(type), Value::OBJECT_VALUE);
		}
		it++;
	}
	return ExpressionResult();
}

ExpressionResult Struct::setMember(const Path *member, Value *value, ContextPtr context, Value **hold) {
	RPNValueType memberType;
	if (!this->definition->hasMember(member->ats(member->size() - 1), &memberType)) {
		return ExpressionResult(
			"Struct " + this->definition->name + " has no member named " + member->ats(member->size() - 1),
			member->getRange(),
			context
		);
	}
	if (this->immutable) {
		return ExpressionResult(
			"Struct " + this->definition->name + " is immutable",
			member->getRange(),
			context
		);
	}
	if (memberType.index() == 0) {
		if (value->getType() != STRUCT) {
			return ExpressionResult(
				"Struct " + this->definition->name + " member " + member->ats(member->size() - 1) + 
				" is of type struct but " + value->getStringType() + " was given",
				value->getRange(),
				context
			);
		}
		Struct *structValue = static_cast<Struct*>(value);
		if (structValue->definition->name != std::get<std::string>(memberType)) {
			return ExpressionResult(
				"Struct " + this->definition->name + " member " + member->ats(member->size() - 1) + 
				" is of type " + std::get<std::string>(memberType) + " but " + std::string(structValue->getStructName()) + " was given",
				value->getRange(),
				context
			);
		}
	} else if (std::get<ValueType>(memberType) != value->getType()) {
		return ExpressionResult(
			"Struct " + this->definition->name + " member " + member->ats(member->size() - 1) + 
			" is of type " + Value::stringType(std::get<ValueType>(memberType)) + " but " + value->getStringType() + " was given",
			value->getRange(),
			context
		);
	}

	Value **memberValue = &this->members->at(member->ats(member->size() - 1));
	if (hold != nullptr) {
		*hold = *memberValue;
		Value::deleteValue(memberValue, Value::OBJECT_VALUE);
	}
	value->setOwner(Value::OBJECT_VALUE);
	*memberValue = value;

	return ExpressionResult();
}

ExpressionResult Struct::getMember(const Path *member, Value *&value, ContextPtr context) {
	if (!this->definition->hasMember(member->ats(member->size() - 1), nullptr)) {
		return ExpressionResult(
			"Struct " + this->definition->name + " has no member named " + member->ats(member->size() - 1),
			member->getRange(),
			context
		);
	}
	value = this->members->at(member->ats(member->size() - 1));
	return ExpressionResult();
}

void Struct::setMember(std::string_view member, Value *value, ContextPtr context) {
	Value **memberValue = &(*this->members)[std::string(member)];
	Value::deleteValue(memberValue, Value::OBJECT_VALUE);
	value->setOwner(Value::OBJECT_VALUE);
	*memberValue = value;
}

Value* Struct::getMember(std::string_view member) {
	return this->members->at(std::string(member));
}

bool Struct::isNumber() const {
	return false;
}

Value *Struct::to(ValueType type, ValueOwner owner) const {
	if (type == STRUCT)
		return this->copy(owner);
	
	throw std::runtime_error("Cannot cast Struct to " + Value::stringType(type));
}

inline Value *Struct::copy(ValueOwner owner) const {
	Struct *copy = new Struct(this->getRange(), this->definition->name, owner);
	copy->members = this->members;
	copy->immutable = this->immutable;
	copy->data = this->data;
	return copy;
}

inline std::string Struct::getStringValue() const {
	std::stringstream stream;
	stream << "Struct " << this->definition->name << " ";
	for (size_t i = 0; i < this->definition->memberOrder.size(); i++) {
		stream << this->definition->memberOrder[i] << " -> ";
		stream << this->members->at(this->definition->memberOrder[i])->getStringValue();
		if (i < this->definition->memberOrder.size() - 1) {
			stream << "; ";
		}
	}
	stream << "End of Struct " << this->definition->name;
	return stream.str();
}

std::string_view Struct::getStructName() const {
	return this->definition->name;
}

void Struct::setData(const std::shared_ptr<void> &data) {
	this->data = data;
}

std::shared_ptr<void> Struct::getData() const {
	return this->data;
}

std::unordered_map<std::string, StructDefinition> Struct::definitions;

void Struct::addStructDefinition(StructDefinition &definition) {
	Struct::definitions[definition.name] = definition;
}

int Struct::getStructMembersCount(std::string_view structName) {
	return Struct::definitions[std::string(structName)].members.size();
}

bool Struct::structExists(std::string_view name) {
	return Struct::definitions.find(std::string(name)) != Struct::definitions.end();
}

StructDefinition Struct::getStructDefinition(std::string_view structName) {
	return Struct::definitions.at(std::string(structName));
}

ExpressionResult Struct::getStruct(const Path *path, Value *&structValue, ContextPtr context) {
	structValue = context->getValue(path->ats(0));
	if (structValue->getType() != STRUCT) {
		return ExpressionResult(
			"Can't access member of non struct",
			path->getRange(),
			context
		);
	}
	ExpressionResult result;
	for (size_t i = 1; i < path->size() - 1; i++) {
		Value *value;
		result = static_cast<Struct *>(structValue)->getMember(
			path, value, context
		);
		if (result.error()) return result;
		if (value->getType() != STRUCT) {
			return ExpressionResult(
				"Can't access member of non struct",
				path->getRange(),
				context
			);
		}
		structValue = value;
	}
	return result;
}


Value *Struct::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot add value to struct");
}

Value *Struct::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot subtract value from struct");
}

Value *Struct::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot multiply struct by value");
}

Value *Struct::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot divide struct by value");
}

Value *Struct::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot mod struct by value");
}

Value *Struct::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot raise struct to power of value");
}


Value *Struct::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare struct to element of type " + Value::stringType(other->getType()));
}

Value *Struct::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare struct to element of type " + Value::stringType(other->getType()));
}

Value *Struct::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare struct to element of type " + Value::stringType(other->getType()));
}

Value *Struct::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	throw std::runtime_error("Cannot compare struct to element of type " + Value::stringType(other->getType()));
}

Value *Struct::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRUCT) {
		return new Bool(true, this->getRange(), Value::INTERPRETER);
	}
	const Struct *otherStruct = static_cast<const Struct*>(other);
	if (this->definition->name != otherStruct->definition->name) {
		return new Bool(true, this->getRange(), Value::INTERPRETER);
	}
	for (auto it = this->members->begin(); it != this->members->end(); it++) {
		auto otherIt = otherStruct->members->find(it->first);
		Value *result = it->second->opne(otherIt->second, range, context);
		if (static_cast<Bool*>(result)->getValue()) {
			return result;
		}
	}
	return new Bool(false, this->getRange(), Value::INTERPRETER);
}

Value *Struct::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRUCT) {
		return new Bool(false, this->getRange(), Value::INTERPRETER);
	}
	const Struct *otherStruct = static_cast<const Struct*>(other);
	if (this->definition->name != otherStruct->definition->name) {
		return new Bool(false, this->getRange(), Value::INTERPRETER);
	}
	for (auto it = this->members->begin(); it != this->members->end(); it++) {
		auto otherIt = otherStruct->members->find(it->first);
		Value *result = it->second->opeq(otherIt->second, range, context);
		if (!static_cast<Bool*>(result)->getValue()) {
			return result;
		}
	}
	return new Bool(true, this->getRange(), Value::INTERPRETER);
}
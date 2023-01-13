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

void StructDefinition::addMember(std::string_view name, ValueType type) {
	std::string nameStr = std::string(name);
	this->members[nameStr] = type;
	this->memberOrder.push_back(nameStr.data());
}

bool StructDefinition::hasMember(std::string_view name, ValueType *memberType) const {
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

void StructDefinition::display(std::ostream &stream) const {
	for (const std::string &key : this->memberOrder) {
		stream << "\t" << key << " -> " << Value::stringType(this->members.at(key)) << std::endl;
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
	definition(&definitions[std::string(name)]) {}

Struct::~Struct() {
	for (auto &pair : this->members) {
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
		ValueType type = this->definition->members.at(key);
		if (!(*it)->isCastableTo(type)) {
			return ExpressionResult(
				"Struct " + this->definition->name + " member " + key + 
				" is of type " + Value::stringType(type) + " but " + (*it)->getStringType() + " was given",
				(*it)->getRange(),
				context
			);
		}
		this->members[key] = (*it)->to(type, Value::OBJECT_VALUE);
		it++;
	}
	return ExpressionResult();
}

ExpressionResult Struct::setMember(const Path *member, Value *value, ContextPtr context, Value **hold) {
	ValueType memberType;
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
	if (memberType != value->getType()) {
		return ExpressionResult(
			"Struct " + this->definition->name + " member " + member->ats(member->size() - 1) + 
			" is of type " + value->getStringType() + " but " + value->getStringType() + " was given",
			value->getRange(),
			context
		);
	}

	Value **memberValue = &this->members[member->ats(member->size() - 1)];
	if (hold != nullptr) {
		*hold = *memberValue;
		delete (*memberValue);
	}
	*memberValue = value;

	return ExpressionResult();
}

ExpressionResult Struct::getMember(const Path *member, Value *&value, ContextPtr context) {
	ValueType memberType;
	if (!this->definition->hasMember(member->ats(member->size() - 1), &memberType)) {
		return ExpressionResult(
			"Struct " + this->definition->name + " has no member named " + member->ats(member->size() - 1),
			member->getRange(),
			context
		);
	}
	value = this->members[member->ats(member->size() - 1)];
	return ExpressionResult();
}

bool Struct::isCastableTo(ValueType type) const {
	return type == STRING || type == STRUCT;
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
	for (const std::string &key : this->definition->memberOrder) {
		copy->members[key] = this->members.at(key)->copy(Value::OBJECT_VALUE);
	}
	return copy;
}

inline std::string Struct::getStringValue() const {
	std::stringstream stream;
	stream << "Struct " << this->definition->name << " ";
	for (size_t i = 0; i < this->definition->memberOrder.size(); i++) {
		stream << this->definition->memberOrder[i] << " -> ";
		stream << this->members.at(this->definition->memberOrder[i])->getStringValue();
		if (i < this->definition->memberOrder.size() - 1) {
			stream << "; ";
		}
	}
	stream << "End of Struct " << this->definition->name;
	return stream.str();
}

void Struct::setData(std::any data) {
	this->data = data;
}

std::any Struct::getData() {
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

ExpressionResult Struct::getStruct(const Path *path, Value *&structValue, ContextPtr context) {
	ExpressionResult result = context->getValue(path->ats(0), path->getRange(), structValue);
	if (result.error()) return result;
	if (structValue->getType() != STRUCT) {
		return ExpressionResult(
			"Can't access member of non struct",
			path->getRange(),
			context
		);
	}
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


operatorResult Struct::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot add element of type " + Value::stringType(other->getType()) + " to struct",
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot subtract element of type " + Value::stringType(other->getType()) + " from struct",
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot multiply struct by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot divide struct by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot mod struct by element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot raise struct to the power of element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}


operatorResult Struct::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare struct to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare struct to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare struct to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair<ExpressionResult, Value*>(
		ExpressionResult(
			"Cannot compare struct to element of type " + Value::stringType(other->getType()),
			range,
			context
		),
		nullptr
	);
}

operatorResult Struct::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRUCT) {
		return std::make_pair(
			ExpressionResult(),
			new Bool(true, this->getRange(), Value::INTERPRETER)
		);
	}
	const Struct *otherStruct = static_cast<const Struct*>(other);
	if (this->definition->name != otherStruct->definition->name) {
		return std::make_pair(
			ExpressionResult(),
			new Bool(true, this->getRange(), Value::INTERPRETER)
		);
	}
	for (auto it = this->members.begin(); it != this->members.end(); it++) {
		auto otherIt = otherStruct->members.find(it->first);
		operatorResult result = it->second->opne(otherIt->second, range, context);
		if (result.first.error()) {
			return result;
		}
		if (static_cast<Bool*>(result.second)->getValue()) {
			return std::make_pair(
				ExpressionResult(),
				result.second
			);
		}
	}
	return std::make_pair(
		ExpressionResult(),
		new Bool(false, this->getRange(), Value::INTERPRETER)
	);
}

operatorResult Struct::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	if (other->getType() != STRUCT) {
		return std::make_pair(
			ExpressionResult(),
			new Bool(false, this->getRange(), Value::INTERPRETER)
		);
	}
	const Struct *otherStruct = static_cast<const Struct*>(other);
	if (this->definition->name != otherStruct->definition->name) {
		return std::make_pair(
			ExpressionResult(),
			new Bool(false, this->getRange(), Value::INTERPRETER)
		);
	}
	for (auto it = this->members.begin(); it != this->members.end(); it++) {
		auto otherIt = otherStruct->members.find(it->first);
		operatorResult result = it->second->opeq(otherIt->second, range, context);
		if (result.first.error()) {
			return result;
		}
		if (!static_cast<Bool*>(result.second)->getValue()) {
			return std::make_pair(
				ExpressionResult(),
				result.second
			);
		}
	}
	return std::make_pair(
		ExpressionResult(),
		new Bool(true, this->getRange(), Value::INTERPRETER)
	);
}
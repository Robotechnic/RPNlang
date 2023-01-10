#include "value/types/struct.hpp"

StructDefinition::StructDefinition(std::string_view name) : name(name) {}
StructDefinition::StructDefinition(const StructDefinition &other) : name(other.name), members(other.members) {}
StructDefinition::StructDefinition(StructDefinition &&other) : name(std::move(other.name)), members(std::move(other.members)) {}

void StructDefinition::operator=(StructDefinition &other) {
	this->name = other.name;
	this->members = other.members;
}

void StructDefinition::addMember(std::string_view name, ValueType type) {
	this->members[std::string(name)] = type;
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
	for (auto it = this->members.begin(); it != this->members.end(); it++) {
		stream << "\t" << it->first << " -> " << it->second << std::endl;
	}
}

std::ostream &operator<<(std::ostream &stream, const StructDefinition &definition) {
	stream << "Struct " << definition.getName() << std::endl;
	definition.display(stream);
	stream << "End of Struct " << definition.getName() << std::endl;
	return stream;
}

Struct::Struct(TextRange range, std::string_view name, ValueOwner owner) : 
	Value(STRUCT, range, owner), 
	definition(&definitions[std::string(name)]) {}

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
	auto it = this->definition->members.begin();
	for (Value *member : members) {
		if (member->getType() != it->second) {
			return ExpressionResult(
				"Struct " + this->definition->name + " member " + it->first + 
				" is of type " + member->getStringType() + " but " + member->getStringType() + " was given",
				member->getRange(),
				context
			);
		}
		this->members[it->first] = member;
		it++;
	}
	return ExpressionResult();
}

ExpressionResult Struct::setMember(Value *name, Value *value, ContextPtr context) {
	ValueType memberType;
	if (!this->definition->hasMember(name->getStringValue(), &memberType)) {
		return ExpressionResult(
			"Struct " + this->definition->name + " has no member named " + name->getStringValue(),
			name->getRange(),
			context
		);
	}
	if (memberType != value->getType()) {
		return ExpressionResult(
			"Struct " + this->definition->name + " member " + name->getStringValue() + 
			" is of type " + value->getStringType() + " but " + value->getStringType() + " was given",
			value->getRange(),
			context
		);
	}

	Value **member = &this->members[name->getStringValue()];
	Value::deleteValue(member, Value::OBJECT_VALUE);
	*member = value;

	return ExpressionResult();
}

ExpressionResult Struct::getMember(Value *name, Value *value, ContextPtr context) {
	ValueType memberType;
	if (!this->definition->hasMember(name->getStringValue(), &memberType)) {
		return ExpressionResult(
			"Struct " + this->definition->name + " has no member named " + name->getStringValue(),
			name->getRange(),
			context
		);
	}
	value = this->members[name->getStringValue()];
	return ExpressionResult();
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
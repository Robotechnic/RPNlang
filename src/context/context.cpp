#include "context/context.hpp"

Context::Context() :
	name(""),
	symbols(),
	type(CONTEXT_TYPE_DEFAULT),
	parent(nullptr)
	{}

Context::Context(std::string name, ContextType type) :
	name(name),
	symbols(),
	type(type),
	parent(nullptr)
	{}

Context::Context(std::string name, Context* parent, ContextType type) :
	name(name),
	symbols(),
	type(type),
	parent(parent)
	{}

Context::Context(std::string name, symbolTable symbols, ContextType type) :
	name(name),
	symbols(symbols),
	type(type),
	parent(nullptr)
	{}

Context::Context(std::string name, symbolTable symbols, Context* parent, ContextType type) :
	name(name),
	symbols(symbols),
	type(type),
	parent(parent)
	{}

void Context::setName(std::string name) {
	this->name = name;
}

std::string Context::getName() const {
	return this->name;
}

void Context::setParent(Context* parent) {
	this->parent = parent;
}

ContextType Context::getType() const {
	return this->type;
}

Context* Context::getParent() const {
	return this->parent;
}

/**
 * @brief set a name and value in the context
 * 
 * @param name the name of the value
 * @param value the value to set
 */
void Context::setValue(const Token &name, Value value) {
	this->symbols[name.getValue()] = value;
}

/**
 * @brief get a value from the context or its parent
 * 
 * @param name the name of the value
 * @param value a reference to the value to set
 * @return ExpressionResult if the value was found
 */
ExpressionResult Context::getValue(const Token &name, Value &value) const {
	std::string nameStr = name.getValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	} else if (this->parent != nullptr) {
		return this->parent->getValue(name, value);
	} else {
		return ExpressionResult("Undefined variable name:" + nameStr, name.getRange());
	}
}
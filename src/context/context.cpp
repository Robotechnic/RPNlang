#include "context/context.hpp"

Context::Context() :
	name(""),
	symbols(),
	type(CONTEXT_TYPE_DEFAULT),
	parent(nullptr)
	{}

Context::Context(const Context &other) :
	name(other.name),
	symbols(other.symbols),
	type(other.type),
	parent(other.parent)
	{}

Context::Context(std::string name, ContextType type) :
	name(name),
	symbols(),
	type(type),
	parent(nullptr)
	{}

Context::Context(std::string name, const Context* parent, ContextType type) :
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

Context::Context(std::string name, symbolTable symbols, const Context* parent, ContextType type) :
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

const Context* Context::getParent() const {
	return this->parent;
}

/**
 * @brief set a name and value in the context
 * 
 * @param name the name of the value
 * @param value the value to set
 */
void Context::setValue(std::string name, Value value) {
	this->symbols[name] = value;
}

void Context::setValue(const Token &name, Value value) {
	this->symbols[name.getValue()] = value;
}

void Context::setValue(const Value &name, Value value) {
	this->symbols[name.getStringValue()] = value;
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
		return ExpressionResult(
			"Undefined variable name:" + nameStr,
			name.getRange(),
			*this
		);
	}
}

ExpressionResult Context::getValue(const Value &name, Value &value) const {
	std::string nameStr = name.getStringValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	} else if (this->parent != nullptr) {
		return this->parent->getValue(name, value);
	} else {
		return ExpressionResult(
			"Undefined variable name:" + nameStr,
			name.getRange(),
			*this
		);
	}
}

/**
 * @brief get the value from the context or its parents but throw an error if it is not found
 * 
 * @param name the name of the value
 * @return Value the desired value
 */
Value Context::getValue(const Token &name) const {
	Value value;
	ExpressionResult result = this->getValue(name, value);
	if (result.error()) {
		throw result.getErrorMessage();
	}
	return value;
}

Value Context::getValue(const Value &name) const {
	Value value;
	ExpressionResult result = this->getValue(name, value);
	if (result.error()) {
		throw result.getErrorMessage();
	}
	return value;
}

/**
 * @brief clear symbol table for the current context
 */
void Context::clear() {
	this->symbols.clear();
}

std::ostream& operator<<(std::ostream& os, const Context& context) {
	if (context.getParent() != nullptr) {
		os << *context.getParent();
	}
	switch (context.getType()) {
		case CONTEXT_TYPE_DEFAULT:
			os << "In : " << context.getName() << std::endl;
			break;
		case CONTEXT_TYPE_FUNCTION:
			os << "In function: " << context.getName() << std::endl;
			break;
		case CONTEXT_TYPE_FILE:
			os << "In file: " << context.getName() << std::endl;
			break;
	}
	return os;
}
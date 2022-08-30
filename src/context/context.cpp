#include "context/context.hpp"


Context::Context(const Context &other) :
	name(other.name),
	filePath(other.filePath),
	symbols(other.symbols),
	type(other.type),
	parent(other.parent),
	child(nullptr)
	{}

Context::Context(const Context *other) :
	name(other->name),
	filePath(other->filePath),
	symbols(other->symbols),
	type(other->type),
	parent(other->parent),
	child(nullptr)
	{}

Context::Context(std::string name, std::string filePath, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(),
	type(type),
	parent(nullptr),
	child(nullptr)
	{}

Context::Context(std::string name, std::string filePath, const Context *parent, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(),
	type(type),
	parent(parent),
	child(nullptr)
	{}

Context::Context(std::string name, std::string filePath, symbolTable symbols, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(symbols),
	type(type),
	parent(nullptr),
	child(nullptr)
	{}

Context::Context(std::string name, std::string filePath, symbolTable symbols, const Context *parent, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(symbols),
	type(type),
	parent(parent),
	child(nullptr)
	{}

Context::~Context() {
	if (this->child != nullptr) {
		delete this->child;
	}
	
	for (auto it = this->symbols.begin(); it != this->symbols.end(); it++) {
		it->second->clean();
		delete it->second;
	}
}

void Context::setName(std::string name) {
	this->name = name;
}

std::string Context::getName() const {
	return this->name;
}

void Context::setFilePath(std::string filePath) {
	this->filePath = filePath;
}

std::string Context::getFilePath() const {
	if ((this->type == CONTEXT_TYPE_FILE || this->type == CONTEXT_TYPE_MODULE) && this->filePath != "<builtin>")
		return this->filePath;
	
	if (this->parent != nullptr)
		return this->parent->getFilePath();
	
	throw std::runtime_error("Context::getFilePath() - Context has no file path");
}

void Context::setParent(const Context *parent) {
	this->parent = parent;
}

void Context::setChild(Context *child) {
	if (this->child != nullptr) {
		delete this->child;
	}
	this->child = child;
}

/**
 * @brief clear the current child to avoid deletion when another child is set
 * 
 */
void Context::clearChild() {
	this->child = nullptr;
}

ContextType Context::getType() const {
	return this->type;
}

const Context *Context::getParent() const {
	return this->parent;
}

Context * Context::getChild() const {
	return this->child;
}
/**
 * @brief set a name and value in the context
 * 
 * @param name the name of the value
 * @param value the value to set
 */
void Context::setValue(std::string name, Value *value) {
	this->symbols[name] = value;
}

void Context::setValue(const Token &name, Value *value) {
	this->symbols[name.getValue()] = value;
}

void Context::setValue(const Value &name, Value *value) {
	this->symbols[name.getStringValue()] = value;
}

/**
 * @brief get a value from the context or its parent
 * 
 * @param name the name of the value
 * @param value a reference to the value to set
 * @return ExpressionResult if the value was found
 */
ExpressionResult Context::getValue(const Token &name, Value *&value) const {
	std::string nameStr = name.getValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	}

	if (this->parent != nullptr)
		return this->parent->getValue(name, value);
	
	return ExpressionResult(
		"Undefined variable name : " + nameStr,
		name.getRange(),
		this
	);
}

ExpressionResult Context::getValue(const Value *name, Value *&value) const {
	std::string nameStr = name->getStringValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	}

	if (this->parent != nullptr)
		return this->parent->getValue(name, value);
	
	return ExpressionResult(
		"Undefined variable name : " + nameStr,
		name->getRange(),
		this
	);
}

ExpressionResult Context::getValue(const Value *path, const std::string name, Value *&value) const {
	if (this->symbols.find(name) != this->symbols.end()) {
		value = this->symbols.at(name)->copy();
		return ExpressionResult();
	}

	if (this->parent != nullptr)
		return this->parent->getValue(path, name, value);
	
	return ExpressionResult(
		"Undefined variable name : " + name,
		path->getRange(),
		this
	);
}

ExpressionResult Context::getValue(const Token &path, const std::string name, Value *&value) const {
	if (this->symbols.find(name) != this->symbols.end()) {
		value = this->symbols.at(name)->copy();
		return ExpressionResult();
	}

	if (this->parent != nullptr)
		return this->parent->getValue(path, name, value);
	
	return ExpressionResult(
		"Undefined variable name : " + name,
		path.getRange(),
		this
	);
}

/**
 * @brief search if a value exist in the context or it's parents
 * 
 * @param name the value to search
 * @return bool if the value exits
 */
bool Context::hasValue(const std::string name) const {
	if (this->symbols.find(name) != this->symbols.end()) return true;
	if (this->parent != nullptr) return this->parent->hasValue(name);
	return false;
}

/**
 * @brief get the value from the context or its parents but throw an error if it is not found
 * 
 * @param name the name of the value
 * @return Value the desired value
 */
Value *Context::getValue(const Token &name) const {
	Value *value;
	ExpressionResult result = this->getValue(name, value);
	if (result.error()) {
		throw result.getErrorMessage();
	}
	return value;
}

Value *Context::getValue(const Value *name) const {
	Value *value;
	ExpressionResult result = this->getValue(name, value);
	if (result.error()) {
		throw result.getErrorMessage();
	}
	return value;
}


Value *Context::getValue(const std::string name) const {
	if (this->symbols.find(name) != this->symbols.end()) {
		return this->symbols.at(name);
	}

	if (this->parent != nullptr)
		return this->parent->getValue(name);
	
	throw std::runtime_error("Undefined variable name '" + name + "'");
}

std::ostream& operator<<(std::ostream& os, const Context *context) {
	if (context->getParent() != nullptr) {
		os << context->getParent();
	}
	switch (context->getType()) {
		case CONTEXT_TYPE_DEFAULT:
			os << "In : ";
			break;
		case CONTEXT_TYPE_FUNCTION:
			os << "In function: ";
			break;
		case CONTEXT_TYPE_FILE:
			os << "In file: ";
			break;
		case CONTEXT_TYPE_MODULE:
			os <<"In module: ";
			break;
	}

	os << context->getName();
	switch (context->getType()) {
		case CONTEXT_TYPE_FILE:
		case CONTEXT_TYPE_MODULE:
			os << " (" << context->getFilePath() << ")";
			break;
		default:
			break;
	}

	std::cout<<std::endl;

	return os;
}
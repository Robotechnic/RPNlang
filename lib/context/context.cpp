#include "context/context.hpp"


Context::Context(const Context &other) :
	name(other.name),
	filePath(other.filePath),
	symbols(other.symbols),
	type(other.type),
	parent(other.parent),
	root(other.root)
	{}

Context::Context(const Context *other) :
	name(other->name),
	filePath(other->filePath),
	symbols(other->symbols),
	type(other->type),
	parent(other->parent),
	root(other->root)
	{}

Context::Context(std::string name, std::string filePath, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(),
	type(type)
	{}

Context::Context(std::string name, std::string filePath, ContextPtr parent, ContextType type, bool root) :
	name(name),
	filePath(filePath),
	symbols(),
	type(type),
	parent(parent)
	{
		if (root) return;
		if (parent->root.use_count() > 0) {
			this->root = parent->root;
		} else {
			this->root = parent;
		}
	}

Context::Context(std::string name, std::string filePath, symbolTable symbols, ContextType type) :
	name(name),
	filePath(filePath),
	symbols(symbols),
	type(type)
	{}

Context::Context(std::string name, std::string filePath, symbolTable symbols, ContextPtr parent, ContextType type, bool root) :
	name(name),
	filePath(filePath),
	symbols(symbols),
	type(type),
	parent(parent)
	{
		if (root) return;
		if (parent->root.use_count() > 0) {
			this->root = parent->root;
		} else {
			this->root = parent;
		}
	}

Context::~Context() {
	for (auto it = this->symbols.begin(); it != this->symbols.end(); it++) {
		delete it->second;
		it->second = nullptr;
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
	if ((
		this->type == CONTEXT_TYPE_FILE || 
		this->type == CONTEXT_TYPE_MODULE || 
		this->type == CONTEXT_TYPE_BUILTIN_MODULE) 
		&& this->filePath != "<builtin>"
	) {
		return this->filePath;
	}
	
	if (this->parent != nullptr)
		return this->parent->getFilePath();
	
	throw std::runtime_error("Context::getFilePath() - Context has no file path");
}

void Context::setParent(ContextPtr parent) {
	this->parent = parent;
}

ContextType Context::getType() const {
	return this->type;
}

ContextPtr Context::getParent() const {
	return this->parent;
}

/**
 * @brief set a name and value in the context
 * 
 * @param name the name of the value
 * @param value the value to set
 */
void Context::setValue(std::string name, Value *value, Value **hold) {
	if (this->symbols.contains(name) && this->symbols[name] != nullptr) {
		if (hold)
			(*hold) = this->symbols[name];
		delete this->symbols[name];
	}
	this->symbols[name] = value;
}

void Context::setValue(const Token *name, Value *value, Value **hold) {
	std::string nameStr = name->getStringValue();
	if (this->symbols.contains(nameStr) && this->symbols[nameStr] != nullptr) {
		if (hold)
			(*hold) = this->symbols[nameStr];
		delete this->symbols[nameStr];
	}
	this->symbols[nameStr] = value;
}

void Context::setValue(const Value *name, Value *value, Value **hold) {
	if (name->getType() != VARIABLE)
		throw std::runtime_error("Context::setValue() - name is not a variable");
	std::string nameStr = name->getStringValue();
	if (this->symbols.contains(nameStr) && this->symbols[nameStr] != nullptr) {
		if (hold)
			(*hold) = this->symbols[nameStr];
		delete this->symbols[nameStr];
	}
	this->symbols[nameStr] = value;
}

/**
 * @brief get a value from the context or its parent
 * 
 * @param name the name of the value
 * @param value a reference to the value to set
 * @return ExpressionResult if the value was found
 */
ExpressionResult Context::getValue(const Value *name, Value *&value) {
	std::string nameStr = name->getStringValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	}

	if (this->root) {
		return this->root->getValue(name, value);
	}

	return ExpressionResult(
		"Undefined variable name : " + nameStr,
		name->getRange(),
		this->shared_from_this()
	);
}

ExpressionResult Context::getValue(const Token *name, Value *&value) {
	std::string nameStr = name->getStringValue();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	}

	if (this->root) {
		return this->root->getValue(name, value);
	}
	
	return ExpressionResult(
		"Undefined variable name : " + nameStr,
		name->getRange(),
		this->shared_from_this()
	);
}

ExpressionResult Context::getModuleValue(std::vector<std::string> path, TextRange range, Value *&value) {
	std::string nameStr = path.back();
	if (this->symbols.find(nameStr) != this->symbols.end()) {
		value = this->symbols.at(nameStr);
		return ExpressionResult();
	}
	
	return ExpressionResult(
		"Undefined variable name : " + nameStr,
		range,
		this->shared_from_this()
	);
}


/**
 * @brief search if a value exist in the context
 * 
 * @param name the value to search
 * @return bool if the value exits
 */
bool Context::hasValue(const std::string name) const {
	return this->symbols.find(name) != this->symbols.end();
}

/**
 * @brief get the value from the context but throw an error if it is not found
 * 
 * @param name the name of the value
 * @return Value the desired value
 */
Value *Context::getValue(const Value *name) {
	Value *value;
	ExpressionResult result = this->getValue(name, value);
	if (result.error()) {
		throw result.getErrorMessage();
	}
	return value;
}

Value *Context::getValue(const std::string &name) {
	return this->symbols.at(name);
}

std::ostream& operator<<(std::ostream& os, const ContextPtr &context) {
	if (context->getParent().use_count() > 0) {
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
		case CONTEXT_TYPE_BUILTIN_MODULE:
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
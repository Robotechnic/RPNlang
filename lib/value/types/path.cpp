#include "value/types/path.hpp"

Path::Path(std::string_view name, TextRange range) : Value(PATH, range, Value::VALUE_TOKEN), path(split(name, '.')) {}
Path::Path(std::vector<std::string> path, TextRange range) : Value(PATH, range, Value::VALUE_TOKEN), path(path) {}
Path::Path(std::vector<std::string> path, std::string name, TextRange range) : Value(PATH, range, Value::VALUE_TOKEN), name(name), path(path) {}

std::vector<std::string> Path::getPath() const {
	return this->path;
}

bool Path::isCastableTo(ValueType type) const {
	return false;
}
bool Path::isNumber() const {
	return false;
}

Value *Path::to(ValueType type, ValueOwner owner) const {
	throw std::runtime_error("Cannot cast path to anything");
}

inline std::string Path::getStringValue() const {
	return this->name;
}

Value* Path::copy(ValueOwner owner) const {
	return new Path(this->path, this->name, this->range);
}

operatorResult Path::opadd(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opsub(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opmul(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opdiv(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opmod(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::oppow(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opgt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opge(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::oplt(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::ople(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opne(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}

operatorResult Path::opeq(const Value *other, const TextRange &range, const ContextPtr &context) const {
	return std::make_pair(
		ExpressionResult(
			"path object just hold path name in memory, replace it by another value to use it",
			range,
			context	
		),
		nullptr
	);
}
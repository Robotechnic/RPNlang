#include "value/types/path.hpp"


Path::Path(std::vector<std::string> path, TextRange range, ValueType type) :
	Value(type, range, Value::VALUE_TOKEN),
	path(path) {}

std::vector<std::string> Path::getPath() const {
	return this->path;
}

std::string_view Path::at(size_t index) const {
	return this->path.at(index).data();
}

std::string Path::ats(size_t index) const {
	return this->path.at(index);
}

size_t Path::size() const {
	return this->path.size();
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
	return join(this->path, '.');
}

Value* Path::copy(ValueOwner owner) const {
	return new Path(this->path, this->range, this->type);
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
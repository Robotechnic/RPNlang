#include "value/types/path.hpp"

Path::Path(std::string name, TextRange range) : Variable(name, range) {
	this->path = split(name, '.');
}

Path::Path(std::vector<std::string> path, TextRange range) : Variable(join(path, '.'), range), path(path) {}

std::vector<std::string> Path::getPath() const {
	return this->path;
}
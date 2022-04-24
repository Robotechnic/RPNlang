#include "textutilities/textutilities.hpp"

std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v) {
	os<<"Vector(";
	for (size_t i = 0; i < v.size(); i++) {
		os << v.at(i);
		if (i < v.size() - 1)
			os << ", ";
	}
	os<<")";

	return os;
}
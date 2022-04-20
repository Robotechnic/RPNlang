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
#include "textutilities/textutilities.hpp"

/**
 * @brief split a string in a vector of strings
 * 
 * @param str the string to split
 * @param delimiter the delimiter to split the string
 * @return std::vector<std::string> the resulting vector
 */
std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}

/**
 * @brief display vector of strings in the output stream
 * 
 * @param os current output stream
 * @param v vector of strings to display
 * @return std::ostream& new output stream
 */
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
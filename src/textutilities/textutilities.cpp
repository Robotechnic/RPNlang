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

/**
 * @brief extract the name of a file from a path
 * 
 * @param path the path to extract the name from
 * @return std::string the name of the file
 */
std::string extractFileName(std::string path) {
	size_t lastSlash = path.find_last_of("/");
	size_t lastDot = path.find_last_of(".");
	if (lastSlash == std::string::npos) {
		return path.substr(0, lastDot - 3);
	}
	return path.substr(lastSlash + 1, lastDot - 3);
}

std::string extractFilePath(std::string path) {
	size_t lastSlash = path.find_last_of("/");
	if (lastSlash == std::string::npos) {
		return "";
	}
	return path.substr(0, lastSlash);
}
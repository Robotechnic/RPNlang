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
 * @brief join a vector of strings in a string
 * 
 * @param str the vector to join
 * @param delimiter the delimiter to join the vector
 * @return std::string the resulting string
 */
std::string join(std::vector<std::string> str, char delimiter) {
	std::string result;
	for (int i = 0; i < str.size(); i++) {
		result += str[i];
		if (i != str.size() - 1) {
			result += delimiter;
		}
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
	std::filesystem::path p(path);
	return p.stem();
}

std::string extractFilePath(std::string path) {
	std::filesystem::path p(path);
	return p.parent_path();
}


bool openFile(std::ifstream &file, std::string fileName, std::string &error) {
	try {
		file.open(fileName);
		if (file.fail()) {
			error = "Failled to open file " + fileName + " : " + std::strerror(errno) + "";
			return false;
		}
	} catch (const std::exception &e) {
		error = "File stream error :" + std::string(e.what());
		return false;
	}

	return true;
}
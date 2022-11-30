#pragma once

#include <string>
#include <vector>

#include "value/types/variable.hpp"
#include "textutilities/textutilities.hpp"

class Path : public Variable {
	public:
		Path(std::string name, TextRange range);
		Path(std::vector<std::string> path, TextRange range);

		std::vector<std::string> getPath() const;
	
	private:
		std::vector<std::string> path;
};
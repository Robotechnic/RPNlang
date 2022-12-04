#! /bin/sh
# This script creates a new module in the current directory.
# Usage: createModule.sh <module name>

path=$(dirname "`realpath $0`")
echo "$path"

if [ $# -ne 1 ]; then
	echo "Usage: createModule.sh <module name>"
	exit 1
fi


# go to the directory where this script is located
path=$(dirname "`realpath $0`")
cd "$path"
mkdir "$1"
cd "./$1"

# create header file
echo '#pragma once' > "$1".hpp
echo '#include "expressionresult/expressionresult.hpp"' >> "$1".hpp
echo '#include "cppmodule/cppmodule.hpp"' >> "$1".hpp
echo 'ExpressionResult loader(CppModule &module);' >> "$1".hpp

# create source file
echo '#include "'"$1"'.hpp"' > "$1".cpp
echo 'ExpressionResult loader(CppModule &module) {' >> "$1".cpp
echo '	ExpressionResult result;' >> "$1".cpp
echo '	// TODO: implement module' >> "$1".cpp
echo '	return result;' >> "$1".cpp
echo '}' >> "$1".cpp

# create MakeLists.txt
echo 'add_library('"$1"' SHARED ${CMAKE_CURRENT_LIST_DIR}/'"$1"'.cpp)' > CMakeLists.txt
echo 'target_link_libraries('"$1"' PRIVATE RPNlangLib)' >> CMakeLists.txt
echo 'add_custom_command(' >> CMakeLists.txt
echo '	TARGET '"$1"' POST_BUILD COMMAND' >> CMakeLists.txt
echo '	mv ${CMAKE_CURRENT_BINARY_DIR}/lib'"$1"'.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/'"$1"'.so' >> CMakeLists.txt
echo ')' >> CMakeLists.txt

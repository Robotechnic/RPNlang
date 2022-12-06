#! /bin/sh
# This script creates a new module in the current directory.
# Usage: createModule.sh <module name>

if [ $# -ne 1 ]; then
	echo "Usage: createModule.sh <module name>"
	exit 1
fi

# Check for valid module name
grep -q "^[a-zA-Z0-9_\-]\+$" <<< "$1"
if [ $? -ne 0 ]; then
  echo "Module name can only contain letters, numbers, dashes and underscores"
  exit 1
fi

# go to the directory where this script is located
path=$(dirname "`realpath "$0"`")
cd "$path"

# Check if module with the same name already exists
if [ -d "$1" ]; then
  echo "Module with the same name already exists"
  exit 1
fi

mkdir "$1"
cd "./$1"

function ask {
	value=""
	echo -n "$1 [$2]: "
	read value
	if [ -z "$value" ]; then
		value=$2
	fi
}

ask "Short description of the module" "''"
description="$value"
ask "Name" `whoami`
name="$value"

# create header file
cat > "$1".hpp << EOF
#pragma once

#include "expressionresult/expressionresult.hpp"
#include "cppmodule/cppmodule.hpp"

ExpressionResult loader(CppModule *module);

ModuleAPI moduleAPI {
	"$1",
	"$description",
	"1.0",
	"$name",
	loader
};
EOF

# create source file
cat > "$1".cpp << EOF
#include "$1.hpp"
ExpressionResult loader(CppModule *module) {
	ExpressionResult result;
	// TODO: implement module
	return result;
}
EOF

# create CMakeLists.txt
cat > "CMakeLists.txt" << EOF
add_library($1 SHARED \${CMAKE_CURRENT_LIST_DIR}/$1.cpp)
target_link_libraries($1 PRIVATE RPNlangLib)
add_custom_command(
	TARGET $1 POST_BUILD COMMAND
	mv \${CMAKE_CURRENT_BINARY_DIR}/lib$1.so \${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/$1.so
)
EOF
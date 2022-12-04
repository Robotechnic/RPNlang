cmake_minimum_required(VERSION 3.23.0)
set(VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -fPIC")

project(RPNlang CXX)

# generate RPNmodule folder in build directory
add_custom_command(
	OUTPUT RPNlang COMMAND
	mkdir -p RPNmodules
)

# import all three config
include(lib/CMakeLists.txt)
include(modules/CMakeLists.txt)
include(src/CMakeLists.txt)
cmake_minimum_required(VERSION 3.23.0)
set(VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -fPIC")
set(EXPORT_COMPILE_COMMANDS ON)

project(RPNlang CXX)

# generate RPNmodule folder in build directory
execute_process(
	COMMAND mkdir -p RPNmodules
)

# import all three config
include(lib/CMakeLists.txt)
include(modules/CMakeLists.txt)
include(src/CMakeLists.txt)

# clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")
if(CLANG_TIDY_EXE)
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
	add_custom_target(
		tidy
		COMMAND ${CLANG_TIDY_EXE} "${PROJECT_SOURCE_DIR}/src/*/*.cpp" --fix -p "${PROJECT_BINARY_DIR}" -fix-errors
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)
else()
	message(STATUS "clang-tidy not found.")
endif()

# clang-format
find_program(CLANG_FORMAT_EXE NAMES "clang-format" DOC "Path to clang-format executable")
if(CLANG_FORMAT_EXE)
	add_custom_target(
		format
		COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.hpp ${PROJECT_SOURCE_DIR}/lib/*/*.cpp ${PROJECT_SOURCE_DIR}/lib/*/*.hpp ${PROJECT_SOURCE_DIR}/modules/*/*.cpp ${PROJECT_SOURCE_DIR}/modules/*/*.hpp
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
else()
	message(STATUS "clang-format not found.")
endif()

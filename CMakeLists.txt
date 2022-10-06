cmake_minimum_required(VERSION 3.23.0)
set(VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")

project(RPNlang CXX)

include(src/CMakeLists.txt)

add_executable(RPNlang ${sources}) 
target_include_directories(RPNlang PUBLIC ${includes})
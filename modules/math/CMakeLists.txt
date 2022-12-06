add_library(math SHARED ${CMAKE_CURRENT_LIST_DIR}/math.cpp)
target_link_libraries(math PRIVATE RPNlangLib)
add_custom_command(
	TARGET math POST_BUILD COMMAND
	mv ${CMAKE_CURRENT_BINARY_DIR}/libmath.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/math.so
)
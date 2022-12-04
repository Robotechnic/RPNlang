add_library(random SHARED ${CMAKE_CURRENT_LIST_DIR}/random.cpp)
target_link_libraries(random PRIVATE RPNlangLib)
add_custom_command(
	TARGET math POST_BUILD COMMAND
	mv ${CMAKE_CURRENT_BINARY_DIR}/librandom.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/random.so
)
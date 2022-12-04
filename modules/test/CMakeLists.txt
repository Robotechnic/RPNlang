add_library(test SHARED ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
target_link_libraries(test PRIVATE RPNlangLib)
add_custom_command(
	TARGET test POST_BUILD COMMAND
	mv ${CMAKE_CURRENT_BINARY_DIR}/libtest.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/test.so
)

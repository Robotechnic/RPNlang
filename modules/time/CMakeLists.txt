add_library(time SHARED ${CMAKE_CURRENT_LIST_DIR}/time.cpp)
target_link_libraries(time PRIVATE RPNlangLib)
add_custom_command(
	TARGET math POST_BUILD COMMAND
	ln -sf ${CMAKE_CURRENT_BINARY_DIR}/libtime.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/time.so
)
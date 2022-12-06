add_library(terminal SHARED ${CMAKE_CURRENT_LIST_DIR}/terminal.cpp)
target_link_libraries(terminal PRIVATE RPNlangLib)
add_custom_command(
	TARGET terminal POST_BUILD COMMAND
	mv ${CMAKE_CURRENT_BINARY_DIR}/libterminal.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/terminal.so
)
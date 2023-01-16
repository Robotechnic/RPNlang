add_library(fs SHARED ${CMAKE_CURRENT_LIST_DIR}/fs.cpp)
target_link_libraries(fs PRIVATE RPNlangLib)
add_custom_command(
	TARGET fs POST_BUILD COMMAND
	ln -sf ${CMAKE_CURRENT_BINARY_DIR}/libfs.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/fs.so
)

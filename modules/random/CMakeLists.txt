set(TARGET random)
add_library(${TARGET} SHARED ${CMAKE_CURRENT_LIST_DIR}/${TARGET}.cpp)
target_link_libraries(${TARGET} PRIVATE RPNlangLib)
add_custom_command(
	TARGET math POST_BUILD COMMAND
	mv ${CMAKE_CURRENT_BINARY_DIR}/lib${TARGET}.so ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules/${TARGET}.so
)
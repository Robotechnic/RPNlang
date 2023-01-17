add_library(terminal SHARED ${CMAKE_CURRENT_LIST_DIR}/terminal.cpp)
target_link_libraries(terminal PRIVATE RPNlangLib)
set_property(
	TARGET terminal PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)
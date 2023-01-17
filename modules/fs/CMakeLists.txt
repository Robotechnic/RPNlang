add_library(fs SHARED ${CMAKE_CURRENT_LIST_DIR}/fs.cpp)
target_link_libraries(fs PRIVATE RPNlangLib)
set_property(
	TARGET fs PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)

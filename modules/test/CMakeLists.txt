add_library(test SHARED ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
target_link_libraries(test PRIVATE RPNlangLib)
set_property(
	TARGET test PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)

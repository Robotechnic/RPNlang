add_library(random SHARED ${CMAKE_CURRENT_LIST_DIR}/random.cpp)
target_link_libraries(random PRIVATE RPNlangLib)
set_property(
	TARGET random PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)
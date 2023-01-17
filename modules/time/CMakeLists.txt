add_library(time SHARED ${CMAKE_CURRENT_LIST_DIR}/time.cpp)
target_link_libraries(time PRIVATE RPNlangLib)
set_property(
	TARGET time PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)

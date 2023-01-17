add_library(math SHARED ${CMAKE_CURRENT_LIST_DIR}/math.cpp)
target_link_libraries(math PRIVATE RPNlangLib)
set_property(
	TARGET math PROPERTY
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RPNmodules
)
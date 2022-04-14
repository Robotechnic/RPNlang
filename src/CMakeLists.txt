list(APPEND includes
	${CMAKE_CURRENT_LIST_DIR}
)

list(APPEND sources 
	${CMAKE_CURRENT_LIST_DIR}/tokens/token.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/value.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/error.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/interpreter.cpp
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)
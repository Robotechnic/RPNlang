include(${CMAKE_CURRENT_LIST_DIR}/tokens/tokens/CMakeLists.txt)


list(APPEND sources
	# utility	
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textutilities.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textrange.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/escapecharacters.cpp
	
	# base types
	${CMAKE_CURRENT_LIST_DIR}/tokens/regex.cpp
	${CMAKE_CURRENT_LIST_DIR}/tokens/token.cpp
	${CMAKE_CURRENT_LIST_DIR}/context/context.cpp
	${CMAKE_CURRENT_LIST_DIR}/expressionresult/expressionresult.cpp
	${CMAKE_CURRENT_LIST_DIR}/value/valuetypes.cpp
	${CMAKE_CURRENT_LIST_DIR}/value/value.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/rpnfunction.cpp
)
include(${CMAKE_CURRENT_LIST_DIR}/value/types/CMakeLists.txt)

list(APPEND sources
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/builtinsrpnfunction.cpp
	${CMAKE_CURRENT_LIST_DIR}/cppmodule/cppmodule.cpp
)

add_library(RPNlangLib STATIC ${sources})
target_include_directories(RPNlangLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

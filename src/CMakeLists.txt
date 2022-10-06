list(APPEND includes
	${CMAKE_CURRENT_LIST_DIR}
)


list(APPEND sources
	# utility	
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textutilities.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textrange.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/escapecharacters.cpp
	
	# base types
	${CMAKE_CURRENT_LIST_DIR}/tokens/token.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/tokens/tokens/CMakeLists.txt)

list(APPEND sources
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/baseblock.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/codeblock.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/line.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/blockqueue.cpp
	${CMAKE_CURRENT_LIST_DIR}/context/context.cpp
	${CMAKE_CURRENT_LIST_DIR}/expressionresult/expressionresult.cpp
	
	# # RPN lang types
	${CMAKE_CURRENT_LIST_DIR}/value/value.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/rpnfunction.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/modules/builtinmodule.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/modules/module.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/userrpnfunction.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/builtinsrpnfunction.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/value/types/CMakeLists.txt)
# include(${CMAKE_CURRENT_LIST_DIR}/modules/CMakeLists.txt)

list(APPEND sources
	# code processing
	${CMAKE_CURRENT_LIST_DIR}/lexer/lexer.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/memory.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/interpreter.cpp

	# # shell
	${CMAKE_CURRENT_LIST_DIR}/shell/colors.cpp
	${CMAKE_CURRENT_LIST_DIR}/shell/shell.cpp

	# main
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)
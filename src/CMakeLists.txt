list(APPEND includes
	${CMAKE_CURRENT_LIST_DIR}
)

list(APPEND sources
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/baseblock.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/codeblock.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/line.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/blockqueue.cpp
	${CMAKE_CURRENT_LIST_DIR}/codeblocks/functionblock.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/builtinmap.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/userrpnfunction.cpp
	${CMAKE_CURRENT_LIST_DIR}/modules/module.cpp
)

list(APPEND sources
	# code processing
	${CMAKE_CURRENT_LIST_DIR}/lexer/lexer.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/memory.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/interpreter.cpp

	# shell
	${CMAKE_CURRENT_LIST_DIR}/shell/colors.cpp
	${CMAKE_CURRENT_LIST_DIR}/shell/shell.cpp

	# main
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(RPNlang ${sources}) 
target_include_directories(RPNlang PUBLIC ${includes})
target_link_libraries(RPNlang RPNlangLib)
list(APPEND includes
	${CMAKE_CURRENT_LIST_DIR}
)

include(${CMAKE_CURRENT_LIST_DIR}/modules/CMakeLists.txt)

list(APPEND sources
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textutilities.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/textrange.cpp
	${CMAKE_CURRENT_LIST_DIR}/textutilities/escapecharacters.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/rpnfunction.cpp
	${CMAKE_CURRENT_LIST_DIR}/tokens/token.cpp
	${CMAKE_CURRENT_LIST_DIR}/expressionresult/expressionresult.cpp
	${CMAKE_CURRENT_LIST_DIR}/modules/builtinmodule.cpp
	${CMAKE_CURRENT_LIST_DIR}/modules/module.cpp
	${CMAKE_CURRENT_LIST_DIR}/value/value.cpp
	${CMAKE_CURRENT_LIST_DIR}/context/context.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/userrpnfunction.cpp
	${CMAKE_CURRENT_LIST_DIR}/rpnfunctions/builtinsrpnfunction.cpp
	${CMAKE_CURRENT_LIST_DIR}/interpreter/interpreter.cpp
	${CMAKE_CURRENT_LIST_DIR}/shell/colors.cpp
	${CMAKE_CURRENT_LIST_DIR}/shell/shell.cpp
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)
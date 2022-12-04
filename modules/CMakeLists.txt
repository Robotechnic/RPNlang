# include all subfolder here

file(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
foreach(child ${children})
	if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
		include(${CMAKE_CURRENT_LIST_DIR}/${child}/CMakeLists.txt)
	endif()
endforeach()
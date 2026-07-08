
add_library (runtime_lib SHARED "")

set_target_properties(runtime_lib PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON
	)

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (runtime_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (runtime_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CoreException.cc
	${CMAKE_CURRENT_SOURCE_DIR}/CoreObject.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SafetyNet.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SdlException.cc
	)

target_link_libraries (runtime_lib PUBLIC
	invaderz-interface-library
	log_lib
	)

if (WIN32)
	target_link_libraries(runtime_lib PUBLIC
		Dbghelp
	)
endif()

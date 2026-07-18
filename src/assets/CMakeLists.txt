
add_library (assets_lib SHARED "")

set_target_properties(assets_lib PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON
	)

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (assets_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (assets_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Asset.cc
	${CMAKE_CURRENT_SOURCE_DIR}/SdlAssetManager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Sound.cc
	${CMAKE_CURRENT_SOURCE_DIR}/Texture.cc
	)

target_link_libraries (assets_lib PUBLIC
	invaderz-interface-library
	runtime_lib
	SDL3::SDL3
	)

if (WIN32)
	target_link_libraries(assets_lib PUBLIC
		Dbghelp
	)
endif()

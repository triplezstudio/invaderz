
add_library (app_lib SHARED "")

set_target_properties(app_lib PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON
	)

if (APPLE)
    get_target_property(_sdl3_inc SDL3::SDL3 INTERFACE_INCLUDE_DIRECTORIES)
    if (_sdl3_inc)
        get_filename_component(_sdl3_inc_parent "${_sdl3_inc}" DIRECTORY)
        set_property(TARGET SDL3::SDL3 APPEND PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES "${_sdl3_inc_parent}")

    endif()
endif()

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (app_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (app_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cc
	${CMAKE_CURRENT_SOURCE_DIR}/EventData.cc
	)

target_link_libraries (app_lib PUBLIC
	invaderz-interface-library
	runtime_lib
	assets_lib
	SDL3::SDL3
	Eigen3::Eigen
	)

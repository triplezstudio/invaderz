
add_library (invaderz_game_lib SHARED "")
set_target_properties(invaderz_game_lib PROPERTIES
		WINDOWS_EXPORT_ALL_SYMBOLS ON
)

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories (invaderz_game_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	)

target_sources (invaderz_game_lib PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cc
	)

target_link_libraries (invaderz_game_lib
	invaderz-interface-library
	runtime_lib
	app_lib
	Eigen3::Eigen
	)

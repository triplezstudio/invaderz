

add_executable (invaderz)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	)

target_sources (invaderz PRIVATE
	main.cc)

target_link_libraries (invaderz PUBLIC
	invaderz-interface-library
	invaderz_game_lib
	app_lib
	)


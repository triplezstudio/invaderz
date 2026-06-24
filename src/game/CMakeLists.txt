
add_executable (invaderz)

target_sources(invaderz PRIVATE main.cc)

target_link_libraries (invaderz PUBLIC
	invaderz-interface-library
	)


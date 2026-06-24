cmake_minimum_required (VERSION 3.28)

project (invaderz LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set (CMAKE_VERBOSE_MAKEFILE OFF)

set (CMAKE_BUILD_RPATH_USE_ORIGIN true)

find_package(Eigen3 CONFIG REQUIRED)

add_library (invaderz-interface-library INTERFACE)
target_compile_options (invaderz-interface-library INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Werror>
)

add_subdirectory (src)

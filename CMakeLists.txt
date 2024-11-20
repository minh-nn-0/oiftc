cmake_minimum_required(VERSION 3.25)

project(oiftc)

add_executable(oiftc 
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/oiftc.cpp)

target_link_libraries(mqa PUBLIC beaver)
target_compile_definitions(mqa PUBLIC
				GAME_PATH="${PROJECT_SOURCE_DIR}")

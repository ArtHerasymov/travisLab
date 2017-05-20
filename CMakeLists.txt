cmake_minimum_required(VERSION 2.8.7)

include_directories(include)
file(GLOB LIB_SOURCES "./*.c" "src/*.c")

project (server)
add_executable(${CMAKE_PROJECT_NAME} ${LIB_SOURCES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_STANDARD 99)
target_link_libraries(${CMAKE_PROJECT_NAME} progbase jansson ssl)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)

SET(CMAKE_C_FLAGS "-std=c99 -Werror -Wall")


project (client)
file(GLOB LIB_SOURCES "client/*.c")
file(GLOB LIB_SOURCES "clientFolder/*.c")
add_executable(${CMAKE_PROJECT_NAME} ${LIB_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} progbase jansson m ssl)

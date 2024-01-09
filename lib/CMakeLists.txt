cmake_minimum_required(VERSION 3.14)

add_subdirectory(glm)
add_library(schrift schrift/schrift.c)

add_library(lib${PROJECT_NAME} INTERFACE)
target_link_libraries(lib${PROJECT_NAME} INTERFACE glm schrift)
target_include_directories(lib${PROJECT_NAME} INTERFACE stb schrift)
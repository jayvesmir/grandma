cmake_minimum_required(VERSION 3.14)

add_subdirectory(glm)
add_subdirectory(cli11)

add_library(lib${PROJECT_NAME} INTERFACE)
target_link_libraries(lib${PROJECT_NAME} INTERFACE glm CLI11)
target_include_directories(lib${PROJECT_NAME} INTERFACE stb)
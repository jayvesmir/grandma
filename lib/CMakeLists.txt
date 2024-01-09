cmake_minimum_required(VERSION 3.14)

add_library(lib${PROJECT_NAME} INTERFACE)
target_include_directories(lib${PROJECT_NAME} INTERFACE stb)
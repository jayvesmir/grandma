cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE PIKSLFLAT_SRC ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

add_executable(${PROJECT_NAME} ${PIKSLFLAT_SRC})
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})
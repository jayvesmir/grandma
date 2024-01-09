cmake_minimum_required(VERSION 3.14)
project(pikslflat)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/image.cpp
    src/pixelflut.cpp
    src/raytracer/grandma.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE lib${PROJECT_NAME})
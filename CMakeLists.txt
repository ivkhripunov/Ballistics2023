cmake_minimum_required(VERSION 3.26)
project(ballistics2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion")

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(third_party)

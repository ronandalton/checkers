cmake_minimum_required(VERSION 3.10)

project(checkers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

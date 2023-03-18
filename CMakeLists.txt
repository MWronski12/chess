cmake_minimum_required(VERSION 3.15)

project(chess VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

add_executable(chess main.cpp)

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

cmake_minimum_required(VERSION 3.15)

project(chess VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    #-Werror
    # -fsanitize=leak
    # -fsanitize=address
    # -fsanitize=undefined
    # -O2 # Enables compiler optimization
)

add_executable(chess main.cpp src/Gui.cc)

# add_executable(Gui src/GuiExample.cc )

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

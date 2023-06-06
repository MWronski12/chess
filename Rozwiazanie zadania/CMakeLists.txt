cmake_minimum_required(VERSION 3.15)

project(chess VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    # -Werror
    -Ofast # Enables compiler optimization
    # -fsanitize=leak
    # -fsanitize=address
    # -fsanitize=undefined
)

add_executable(chess main.cpp src/Gui.cc)

# # Define a custom target to install packages
# add_custom_target(install_packages
#     COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/install-packages.sh
#     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/install-packages.sh
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# )

# # Add a dependency on the custom target to your main target
# add_dependencies(chess install_packages)

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

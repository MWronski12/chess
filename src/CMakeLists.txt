add_library(engine Piece.cc Move.cc Board.cc Movegen.cc Evaluation.cc Search.cc Engine.cc Gui.cc)

include(FetchContent)
FetchContent_Declare(SFML
GIT_REPOSITORY https://github.com/SFML/SFML.git
GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

target_link_libraries(chess engine sfml-graphics sfml-window sfml-system)

# target_link_libraries(chess PRIVATE sfml-graphics)
# target_link_libraries(chess PRIVATE sfml-window)
# target_link_libraries(chess PRIVATE sfml-system)
install (TARGETS chess)

target_link_libraries(Gui PRIVATE sfml-graphics)
target_link_libraries(Gui PRIVATE sfml-window)
target_link_libraries(Gui PRIVATE sfml-system)
# if (WIN32 AND BUILD_SHARED_LIBS)
#     add_custom_command(TARGET CMakeSFMLProject POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
# endif()
install(TARGETS Gui)

# To build static libraries?
# unset(BUILD_SHARED_LIBS)
# You CMake code isn't how you find SFML.

# Like mentioned in this thread you really just need to have the following three statements for linking SFML statically:

# set(SFML_STATIC_LIBRARIES TRUE)
# find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
# ...
# target_link_libraries(SFMLTest sfml-graphics sfml-audio)

# This requires that you set SFML_DIR to the directory where the SFML config files are located (usually SFML/lib/cmake/SFML).

# Note also that SFML will automatically look for its dependencies and link them, so you don't really need to search for freetype in your own CMake script.

include_directories (/usr/include/freetype2)


add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
   # -Werror
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
    -fsanitize=thread
    -fsanitize=undefined
    # -O2 # Enables compiler optimization
)


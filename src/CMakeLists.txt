add_library(engine Piece.cc MoveContent.cc Board.cc PieceMoves.cc Movegen.cc Engine.cc Evaluation.cc Search.cc)
# target_link_libraries(chess engine)
target_link_libraries(chess engine sfml-graphics sfml-window sfml-system)



include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

install(TARGETS chess)

# target_link_libraries(Gui PRIVATE sfml-graphics)
# target_link_libraries(Gui PRIVATE sfml-window)
# target_link_libraries(Gui PRIVATE sfml-system)
# install(TARGETS Gui)
# if (WIN32 AND BUILD_SHARED_LIBS)
#     add_custom_command(TARGET CMakeSFMLProject POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
# endif()

include_directories (/usr/include/freetype2)
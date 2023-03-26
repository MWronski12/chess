add_library(engine piece.cpp move.cpp board.cpp movegen.cpp)
target_link_libraries(chess engine)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
    -fsanitize=thread
    -fsanitize=undefined
    # -O2 # Enables compiler optimization
)

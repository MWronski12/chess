Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests Piece_test.cc Move_test.cc Board_test.cc)
target_link_libraries(tests engine)
target_link_libraries(tests Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

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
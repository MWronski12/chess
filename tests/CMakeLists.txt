Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests Search_test.cc Piece_test.cc Board_test.cc PieceMoves_test.cc Movegen_test.cc Evaluation_test.cc )
target_link_libraries(tests engine)
target_link_libraries(tests Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

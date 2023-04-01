add_library(engine Piece.cc Move.cc Board.cc Movegen.cc Engine.cc)
target_link_libraries(chess engine)

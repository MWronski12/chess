add_library(engine Piece.cc Move.cc Board.cc PieceMoves.cc Movegen.cc Engine.cc)
target_link_libraries(chess engine)

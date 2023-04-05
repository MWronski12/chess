add_library(engine Piece.cc MoveContent.cc Board.cc PieceMoves.cc Movegen.cc Engine.cc Evaluation.cc Search.cc)
target_link_libraries(chess engine)

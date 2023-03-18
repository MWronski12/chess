#include "move.hpp"

MoveContent::MoveContent(Square src, Square dest, PieceType promotion)
    : src(src), dest(dest), promotion(promotion), pieceMoving(EMPTY), pieceTaken(EMPTY),
      isEnPassantCapture(false), score(0) {}

// TODO
std::string MoveContent::toPGN() { return "O-O-O"; };

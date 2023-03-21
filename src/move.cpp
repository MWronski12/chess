#include "move.hpp"

MoveContent::MoveContent(SquareIndex src, SquareIndex dest, PieceType promotion)
    : src(src), dest(dest), promotion(promotion), pieceMoving(EMPTY), pieceTaken(EMPTY),
      isEnPassantCapture(false), score(0) {}

MoveContent::MoveContent()
    : src(-1), dest(-1), promotion(EMPTY), pieceMoving(EMPTY), pieceTaken(EMPTY),
      isEnPassantCapture(false), score(0) {}

// TODO
std::string MoveContent::toPGN() const { return "O-O-O"; }

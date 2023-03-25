#include "move.hpp"

MoveContent::MoveContent(SquareIndex src, SquareIndex dest, PieceType promotion)
    : src(src), dest(dest), promotion(promotion), pieceMoving(EMPTY), pieceTaken(EMPTY),
      isEnPassantCapture(false), score(0) {}

MoveContent::MoveContent()
    : src(NULL_SQUARE), dest(NULL_SQUARE), promotion(EMPTY), pieceMoving(EMPTY), pieceTaken(EMPTY),
      isEnPassantCapture(false), score(0) {}

// TODO
std::string MoveContent::toPGN() const { return "O-O-O"; }

#ifndef MOVE_HPP
#define MOVE_HPP

#include "piece.hpp"

#include <string>

/* ---------------------------- MoveContent class --------------------------- */
class MoveContent {
public:
  MoveContent();
  MoveContent(SquareIndex src, SquareIndex dest, PieceType promotion);

  SquareIndex src;
  SquareIndex dest;
  PieceType promotion;
  PieceType pieceMoving;
  PieceType pieceTaken;
  bool isEnPassantCapture;
  int score;

  std::string toPGN() const;
};

#endif

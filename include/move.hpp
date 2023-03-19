#ifndef MOVE_HPP
#define MOVE_HPP

#include "piece.hpp"

#include <string>

/* ---------------------------- MoveContent class --------------------------- */
class MoveContent {
public:
  MoveContent();
  MoveContent(Square src, Square dest, PieceType promotion);

  Square src;
  Square dest;
  PieceType promotion;
  PieceType pieceMoving;
  PieceType pieceTaken;
  bool isEnPassantCapture;
  int score;

  std::string toPGN() const;
};

#endif

#include "piece.hpp"

#include <string>

class MoveContent {
public:
  MoveContent() = delete;
  MoveContent(Square src, Square dest, PieceType promotion);

  Square src;
  Square dest;
  PieceType promotion;
  PieceType pieceMoving;
  PieceType pieceTaken;
  bool isEnPassantCapture;
  int score;

  std::string toPGN();
};
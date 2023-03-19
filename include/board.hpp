#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <memory>

#include "move.hpp"
#include "piece.hpp"

/* -------------------------- starting position map ------------------------- */
const PieceType STARTING_POSITION[64] = {
    ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK,  PAWN,  PAWN,   PAWN,   PAWN,  PAWN,
    PAWN,  PAWN,   PAWN,   EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY,
    EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, EMPTY,  EMPTY,  EMPTY, EMPTY, PAWN,   PAWN,   PAWN,  PAWN,
    PAWN,  PAWN,   PAWN,   PAWN,  ROOK,  KNIGHT, BISHOP, QUEEN, KING,  BISHOP, KNIGHT, ROOK};

/* ------------------------------- Board class ------------------------------ */
class Board {
public:
  Board();
  Board(std::string FENString);

  // getters for values that are changed only by class methods
  bool whiteHasCastled() const;
  bool blackHasCastled() const;
  Square getEnPassantSquare() const;
  std::array<Piece, 64> getSquares() const;

  // values that change
  PieceColor sideToMove;
  MoveContent lastMove;
  bool whiteIsChecked;
  bool blackIsChecked;
  bool whiteIsCheckMated;
  bool blackIsCheckMated;
  bool staleMate;
  int score;

  void makeMove(Square src, Square dest, PieceType promotion);
  std::string toFEN() const;

private:
  // Available by getters
  bool _whiteHasCastled;
  bool _blackHasCastled;
  Square _enPassantSquare;
  std::array<std::unique_ptr<Piece>, 64> _squares;

  // Only used by class methods
  int _fiftyMoveCounter;
  int _threefoldRepetitionCounter;
};

#endif

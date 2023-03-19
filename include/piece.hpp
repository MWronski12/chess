#ifndef PIECE_HPP
#define PIECE_HPP

#include <inttypes.h>
#include <vector>

/* -------------------------------- constants ------------------------------- */
auto const PAWN_VALUE = 100;
auto const KNIGHT_VALUE = 300;
auto const BISHOP_VALUE = 325;
auto const ROOK_VALUE = 500;
auto const QUEEN_VALUE = 900;
auto const KING_VALUE = 32767;

auto const PAWN_ACTION_VALUE = 6;
auto const KNIGHT_ACTION_VALUE = 3;
auto const BISHOP_ACTION_VALUE = 3;
auto const ROOK_ACTION_VALUE = 2;
auto const QUEEN_ACTION_VALUE = 1;
auto const KING_ACTION_VALUE = 1;

/* ---------------------------------- enums --------------------------------- */
enum PieceColor { WHITE, BLACK };

enum PieceType {
  EMPTY,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  KING,
  QUEEN,
};

using Square = uint8_t;

/* ------------------------------- Piece class ------------------------------ */
class Piece {
public:
  // constructors
  Piece() = delete;
  Piece(Piece &&otherPiece);
  Piece(PieceColor color, PieceType type, bool hasMoved);

  // getters for values that never change
  PieceType getType() const;
  PieceColor getColor() const;
  int getValue() const;
  int getActionValue() const;

  // values that change
  bool hasMoved;
  int attackedValue;
  int defendedValue;
  std::vector<Square> validMoves;

private:
  PieceType _type;
  PieceColor _color;
  int _value;
  int _actionValue;

  // only called in constructors
  void setPieceValue();
  void setPieceActionValue();
};

#endif

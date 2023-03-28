#include "Piece.h"

Piece::Piece(PieceColor color, PieceType type, bool hasMoved)
    : type(type), hasMoved(hasMoved), attackedValue(0), defendedValue(0), _color(color) {
  setPieceValue();
  setPieceActionValue();
}

PieceColor Piece::getColor() const { return _color; }
int Piece::getValue() const { return _value; }
int Piece::getActionValue() const { return _actionValue; }

void Piece::setPieceValue() {
  switch (type) {
  case PAWN:
    _value = PAWN_VALUE;
    break;

  case KNIGHT:
    _value = KNIGHT_VALUE;
    break;

  case BISHOP:
    _value = BISHOP_VALUE;
    break;

  case ROOK:
    _value = ROOK_VALUE;
    break;

  case QUEEN:
    _value = QUEEN_VALUE;
    break;

  case KING:
    _value = KING_VALUE;

  default:
    break;
  }
}

void Piece::setPieceActionValue() {
  switch (type) {
  case PAWN:
    _actionValue = PAWN_ACTION_VALUE;
    break;

  case KNIGHT:
    _actionValue = KNIGHT_ACTION_VALUE;
    break;

  case BISHOP:
    _actionValue = BISHOP_ACTION_VALUE;
    break;

  case ROOK:
    _actionValue = ROOK_ACTION_VALUE;
    break;

  case QUEEN:
    _actionValue = QUEEN_ACTION_VALUE;
    break;

  case KING:
    _actionValue = KING_ACTION_VALUE;

  default:
    break;
  }
}

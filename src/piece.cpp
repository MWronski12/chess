#include "piece.hpp"

Piece::Piece(PieceColor color, PieceType type, bool hasMoved)
    : _type(type), _color(color), hasMoved(hasMoved), attackedValue(0), defendedValue(0) {
  setPieceValue();
  setPieceActionValue();
}

PieceType Piece::getType() { return _type; }
PieceColor Piece::getColor() { return _color; }
int Piece::getValue() { return _value; }
int Piece::getActionValue() { return _actionValue; }

void Piece::setPieceValue() {
  switch (_type) {
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
  switch (_type) {
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

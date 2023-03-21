#include "piece.hpp"

Piece::Piece(Piece &piece) : _type(piece._type), _color(piece._color) {}

Piece::Piece(Piece &&otherPiece)
    : hasMoved(otherPiece.hasMoved), attackedValue(0), defendedValue(0),
      _type(otherPiece.getType()), _color(otherPiece.getColor()) {
  setPieceValue();
  setPieceActionValue();
}

Piece::Piece(PieceColor color, PieceType type, bool hasMoved)
    : hasMoved(hasMoved), attackedValue(0), defendedValue(0), _type(type), _color(color) {
  setPieceValue();
  setPieceActionValue();
}

PieceType Piece::getType() const { return _type; }
PieceColor Piece::getColor() const { return _color; }
int Piece::getValue() const { return _value; }
int Piece::getActionValue() const { return _actionValue; }

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

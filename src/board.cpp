#include "board.hpp"

Board::Board()
    : sideToMove(WHITE), lastMove(MoveContent()), whiteIsChecked(false), blackIsChecked(false),
      whiteIsCheckMated(false), blackIsCheckMated(false), staleMate(false), score(0),
      _whiteHasCastled(false), _blackHasCastled(false), _enPassantSquare(-1), _squares({nullptr}),
      _fiftyMoveCounter(0), _threefoldRepetitionCounter(0) {

  for (int i = 0; i < 64; i++) {
    if (i > 15 && i < 48) {
      continue;
    }

    PieceColor color;
    PieceType type;

    if (i <= 15) {
      color = WHITE;
    } else {
      color = BLACK;
    }
    _squares[i] = std::make_unique<Piece>(Piece(color, STARTING_POSITION[i], false));
  }
}
#include "board.hpp"

#include <stdexcept>

/* -------------------------------------------------------------------------- */
/*                                Constructors                                */
/* -------------------------------------------------------------------------- */

// Creates a board with a starting position
Board::Board()
    : sideToMove(WHITE), lastMove(MoveContent()), whiteIsChecked(false), blackIsChecked(false),
      whiteIsCheckMated(false), blackIsCheckMated(false), staleMate(false), score(0),
      squares({nullptr}), _whiteHasCastled(false), _blackHasCastled(false), _enPassantSquare(-1),
      _fiftyMoveCounter(0), _threefoldRepetitionCounter(0) {

  for (int i = 0; i < 64; i++) {

    if (STARTING_POSITION[i] == EMPTY) {
      continue;
    }

    PieceColor color;
    PieceType type;

    if (i < 16) { // Black side squares are low indieces
      color = BLACK;
    } else if (i > 47) { // White side squares are high indieces
      color = WHITE;
    }
    squares[i] = std::make_unique<Piece>(Piece(color, STARTING_POSITION[i], false));
  }
}

/* -------------------------------------------------------------------------- */
/*                                   getters                                  */
/* -------------------------------------------------------------------------- */

bool Board::whiteHasCastled() const { return _whiteHasCastled; }
bool Board::blackHasCastled() const { return _blackHasCastled; }
Square Board::getEnPassantSquare() const { return _enPassantSquare; }

/* -------------------------------------------------------------------------- */
/*                         makeMove and helper methods                        */
/* -------------------------------------------------------------------------- */

// Changes state of the board according to the move being made
void Board::makeMove(Square src, Square dest, PieceType promotion) {

  validateMove(src, dest);

  // Record move details
  lastMove.src = src;
  lastMove.dest = dest;
  lastMove.pieceMoving = squares[src]->getType();
  lastMove.pieceTaken = squares[dest]->getType();
  lastMove.promotion = promotion;
  lastMove.isEnPassantCapture = false;

  // Detect type of move and act accordingly
  // Pawn move by 2
  if (squares[src]->getType() == PAWN && abs(src - dest) == 16) {
    recordEnPassant(src, dest);
    handleNormalMove(src, dest);
  }
  // En passant capture
  else if (enPassantIsAvailable()) {
    if (squares[src]->getType() == PAWN && dest == getEnPassantSquare()) {
      handleEnPassant(src, dest);
    }
  }
  // Castling
  else if (squares[src]->getType() == KING && abs(src - dest) == 2) {
    handleCastling(src, dest);
  }
  // Pawn promotion
  else if (squares[src]->getType() == PAWN && (dest < 8 || dest > 55)) {
    handlePromotion(src, dest, promotion);
  }
  // Regular move
  else {
    handleNormalMove(src, dest);
  }

  squares[dest]->hasMoved = true;

  // Switch sideToMove
  if (sideToMove == WHITE) {
    sideToMove = BLACK;
  } else {
    sideToMove = WHITE;
  }
}

bool Board::enPassantIsAvailable() const {
  if (_enPassantSquare == -1) {
    return false;
  }
  return true;
}

void Board::validateMove(Square src, Square dest) const {
  // Validate square indices
  if (src < 0 || src > 63 || dest < 0 || dest > 63) {
    throw std::invalid_argument("Invalid square indices!");
  }

  // Validate source square piece
  if (!squares[src]) {
    throw std::logic_error("Source square must be occupied by a piece!");
  } else if (sideToMove != squares[src]->getColor()) {
    throw std::logic_error(
        "Source square must be occupied by a piece owned by the player whose turn it is!");
  }

  // Validate dest square piece
  if (squares[dest]) {
    if (sideToMove == squares[dest]->getColor()) {
      throw std::logic_error("Capturing allied piece is not allowed!");
    }
  }
};

// Sets en passant square according to destination square
void Board::recordEnPassant(Square src, Square dest) {
  if (squares[src]->getColor() == WHITE) {
    _enPassantSquare = dest - 8;
  } else if (squares[src]->getColor() == BLACK) {
    _enPassantSquare = dest + 8;
  }
}

void Board::handleEnPassant(Square src, Square dest) {
  if (squares[src]->getColor() == WHITE) {
    squares[dest + 8].reset();
  } else if (squares[src]->getColor() == BLACK) {
    squares[dest - 8].reset();
  }
  squares[src].swap(squares[dest]);
}

void Board::handleCastling(Square src, Square dest) {
  // White castle king side
  if (src == 60) {
    // squares[src].swap(squares[])
  };
}
// void Board::handlePromotion(Square src, Square dest, PieceType promotion);
// void Board::handleNormalMove(Square src, Square dest);
#include "board.hpp"

#include <iostream>
#include <stdexcept>

/* -------------------------------------------------------------------------- */
/*                                Constructors                                */
/* -------------------------------------------------------------------------- */

// Creates a board with a starting position
Board::Board()
    : sideToMove(WHITE), lastMove(MoveContent()), whiteIsChecked(false), blackIsChecked(false),
      whiteIsCheckMated(false), blackIsCheckMated(false), staleMate(false), score(0),
      squares({std::nullopt}), _whiteHasCastled(false), _blackHasCastled(false),
      _enPassantSquare(-1), _fiftyMoveCounter(0), _threefoldRepetitionCounter(0) {

  for (int i = 0; i < 64; i++) {

    if (STARTING_POSITION[i] == EMPTY) {
      continue;
    }

    PieceColor color;

    if (i < 16) { // Black side squares are low indieces
      color = BLACK;
    } else if (i > 47) { // White side squares are high indieces
      color = WHITE;
    }

    squares[i] = std::make_optional<Piece>(color, STARTING_POSITION[i], false);
  }
}

/* -------------------------------------------------------------------------- */
/*                                   getters                                  */
/* -------------------------------------------------------------------------- */

bool Board::whiteHasCastled() const { return _whiteHasCastled; }
bool Board::blackHasCastled() const { return _blackHasCastled; }
SquareIndex Board::getEnPassantSquare() const { return _enPassantSquare; }

/* -------------------------------------------------------------------------- */
/*                               makeMove methods                             */
/* -------------------------------------------------------------------------- */

void Board::makeMove(SquareIndex src, SquareIndex dest, PieceType promotion) {

  validateMove(src, dest);

  PieceType pieceMoving = squares[src] ? squares[src]->type : EMPTY;
  PieceType pieceTaken = squares[dest] ? squares[dest]->type : EMPTY;

  // Record move details
  lastMove.src = src;
  lastMove.dest = dest;
  lastMove.pieceMoving = pieceMoving;
  lastMove.pieceTaken = pieceTaken;
  lastMove.promotion = promotion;
  lastMove.isEnPassantCapture = false;

  // Detect special move scenario and handle extra behaviour
  // Pawn move by 2
  if (pieceMoving == PAWN && abs(src - dest) == 16) {
    recordEnPassant(src, dest);
  }
  // En passant capture
  else if (enPassantIsAvailable() && pieceMoving == PAWN && dest == getEnPassantSquare()) {
    handleEnPassant(src, dest);
  }
  // Castling
  else if (pieceMoving == KING && abs(src - dest) == 2) {
    handleCastling(src, dest);
  }
  // Pawn promotion
  else if (pieceMoving == PAWN && (dest < 8 || dest > 55)) {
    handlePromotion(src, promotion);
  }

  // Handle behaviour common to every move
  squares[dest] = std::move(squares[src]);
  squares[src] = std::nullopt;
  squares[dest]->hasMoved = true;

  // Switch sideToMove
  sideToMove = sideToMove == WHITE ? BLACK : WHITE;
}

// D2D4 notation (D2D4Q for promotion)
void Board::makeMove(std::string move) {

  // Calculate and validate SquareIndex
  int src = 8 * (8 - (int(move[1]) - 48)) + (int(toupper(move[0])) - 65);
  int dest = 8 * (8 - (int(move[3]) - 48)) + (int(toupper(move[2])) - 65);
  if (src < 0 || src > 63 || dest < 0 || dest > 63) {
    throw std::invalid_argument("Invalid square notation!");
  }

  PieceType promotion;

  // Normal move like d2d4
  if (move.size() == 4) {
    makeMove(src, dest, EMPTY);
  }
  // Promotion move like d7d8q
  else if (move.size() == 5) {
    std::cout << toupper(move[4]) << "\n\n";
    switch (toupper(move[4])) {
    case 'Q':
      promotion = QUEEN;
      break;

    case 'R':
      promotion = ROOK;
      break;

    case 'B':
      promotion = BISHOP;
      break;

    case 'N':
      promotion = KNIGHT;
      break;

    default:
      throw std::invalid_argument("Invalid promotion type!");
      break;
    }
    makeMove(src, dest, promotion);
  }
  // Invalid move
  else {
    throw std::invalid_argument("Invalid move notation!");
  }
}

/* -------------------------------------------------------------------------- */
/*                           makeMove helper methods                          */
/* -------------------------------------------------------------------------- */
// Returns true if enPassant is available
bool Board::enPassantIsAvailable() const {
  if (_enPassantSquare == NULL_SQUARE) {
    return false;
  }
  return true;
}

// Validates if correct pieces stand on the src and dest squares according to the side to move
void Board::validateMove(SquareIndex src, SquareIndex dest) const {
  // Validate square indices
  if (src > 63 || dest > 63) {
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
}

// Sets en passant square according to destination square
void Board::recordEnPassant(SquareIndex src, SquareIndex dest) {
  if (squares[src]->getColor() == WHITE) {
    _enPassantSquare = dest - 8;
  } else if (squares[src]->getColor() == BLACK) {
    _enPassantSquare = dest + 8;
  }
}

// Clears enPassant square
void Board::handleEnPassant(SquareIndex src, SquareIndex dest) {
  if (squares[src]->getColor() == WHITE) {
    squares[dest + 8] = std::nullopt;
  } else if (squares[src]->getColor() == BLACK) {
    squares[dest - 8] = std::nullopt;
  }
  squares[src].swap(squares[dest]);
}

// Moves the rook
void Board::handleCastling(SquareIndex src, SquareIndex dest) {
  // white castle
  if (src == 60) {
    // king side
    if (dest == 62) {
      squares[61] = std::move(squares[63]);
      squares[61]->hasMoved = true;
      squares[63] = std::nullopt;
    }
    // queen side
    if (dest == 58) {
      squares[59] = std::move(squares[56]);
      squares[59]->hasMoved = true;
      squares[56] = std::nullopt;
    }
    _whiteHasCastled = true;
  }
  // black castle
  else if (src == 4) {
    // king side
    if (dest == 6) {
      squares[5] = std::move(squares[7]);
      squares[5]->hasMoved = true;
      squares[7] = std::nullopt;
    }
    // queen side
    else if (dest == 2) {
      squares[3] = std::move(squares[0]);
      squares[3]->hasMoved = true;
      squares[0] = std::nullopt;
    }
  }
  _blackHasCastled = true;
}

// Changes type of promoted piece
void Board::handlePromotion(SquareIndex src, PieceType promotion) {
  squares[src]->type = promotion;
}

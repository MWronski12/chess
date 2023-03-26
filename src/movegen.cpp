#include "movegen.hpp"

static SquareIndex Position(SquareIndex col, SquareIndex row) { return col + (row * 8); }

// Private constructor to prevent creation of instances from outside the class.
PieceMoves::PieceMoves() {
  generateWhitePawnMoves();
  generateBlackPawnMoves();
  generateKingMoves();
  generateBishopMoves();
  generateRookMoves();
  generateQueenMoves();
  generateKingMoves();
}

void PieceMoves::generateWhitePawnMoves() {
  for (int index = 0; index <= 64; index++) {
    // Pawns cannot stand on 1st and 8th ranks
    if (index < 8 || index > 55) {
      whitePawnMoves[index].push_back(NULL_SQUARE);
      continue;
    }

    int x = (int)(index % 8);
    int y = (int)((index / 8));

    // Diagonal Kill
    if (x < 7 && y > 0) {
      whitePawnMoves[index].push_back(index - 8 - 1);
    }
    if (x > 0 && y > 0) {
      whitePawnMoves[index].push_back(index - 8 + 1);
    }

    // One Forward
    whitePawnMoves[index].push_back(index - 8);

    //  Starting Position we can jump 2
    if (y == 6) {
      whitePawnMoves[index].push_back(index - 16);
    }
  }
}

void PieceMoves::generateBlackPawnMoves() {
  for (SquareIndex index = 0; index <= 64; index++) {
    // Pawns cannot stand on 1st and 8th ranks
    if (index < 8 || index > 55) {
      blackPawnMoves[index].push_back(NULL_SQUARE);
      continue;
    }

    SquareIndex x = (SquareIndex)(index % 8);
    SquareIndex y = (SquareIndex)((index / 8));

    // Diagonal Kill
    if (x < 7 && y > 0) {
      blackPawnMoves[index].push_back(index + 8 - 1);
    }
    if (x > 0 && y > 0) {
      blackPawnMoves[index].push_back(index + 8 + 1);
    }

    // One Forward
    blackPawnMoves[index].push_back(index + 8);

    //  Starting Position we can jump 2
    if (y == 6) {
      blackPawnMoves[index].push_back(index + 16);
    }
  }
}

void PieceMoves::generateKnightMoves() {
  for (SquareIndex y = 0; y < 8; y++) {
    for (SquareIndex x = 0; x < 8; x++) {
      SquareIndex index = (SquareIndex)(y + (x * 8));

      SquareIndex move;

      if (y < 6 && x > 0) {
        move = Position((y + 2), (x - 1));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y > 1 && x < 7) {
        move = Position((y - 2), (x + 1));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y > 1 && x > 0) {
        move = Position((y - 2), (x - 1));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y < 6 && x < 7) {
        move = Position((y + 2), (x + 1));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y > 0 && x < 6) {
        move = Position((y - 1), (x + 2));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y < 7 && x > 1) {
        move = Position((y + 1), (x - 2));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
      if (y > 0 && x > 1) {
        move = Position((y - 1), (x - 2));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }

      if (y < 7 && x < 6) {
        move = Position((y + 1), (x + 2));
        if (move < 64) {
          knightMoves[index].push_back(move);
        }
      }
    }
  }
}

void PieceMoves::generateBishopMoves() {}
void PieceMoves::generateRookMoves() {}
void PieceMoves::generateQueenMoves() {}
void PieceMoves::generateKingMoves() {}
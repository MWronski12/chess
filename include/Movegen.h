#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "Piece.h"
#include <array>
#include <iostream>

#include <vector>

// Interface for PieceMoves generator
class PieceMovesInterface {
public:
  virtual void start(PieceColor color, PieceType piece, SquareIndex src) = 0;
  virtual bool hasNextMove() = 0;
  virtual SquareIndex getNextMove() = 0;
  virtual void skipRay() = 0;
};

// Piece moves generator using nested vectors
// It is readable, but can loose some performance
class PieceMovesNestedLists : public PieceMovesInterface {
public:
  static std::array<std::vector<std::vector<SquareIndex>>, 64> whitePawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> blackPawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> knightMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> bishopMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> rookMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> queenMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> kingMoves;

  // Set correct move ray list pointer and reset indices
  void start(PieceColor color, PieceType piece, SquareIndex src) override {
    switch (piece) {
    case PAWN:
      _currentListPointer = color == WHITE ? &whitePawnMoves[src] : &blackPawnMoves[src];
      break;

    case KNIGHT:
      _currentListPointer = &knightMoves[src];
      break;

    case BISHOP:
      _currentListPointer = &bishopMoves[src];
      break;

    case ROOK:
      _currentListPointer = &rookMoves[src];
      break;

    case QUEEN:
      _currentListPointer = &queenMoves[src];
      break;

    case KING:
      _currentListPointer = &kingMoves[src];
      break;

    default:
      throw std::logic_error("Invalid piece requested!");
      break;
    }
  }

  // Should be called before getNextMove()
  bool hasNextMove() override {
    if (_rayIndex >= _currentListPointer->size()) {
      return false;
    }
    return true;
  }

  SquareIndex getNextMove() override {
    _moveIndex++;
    if (_moveIndex == (_currentListPointer + _rayIndex)->size()) {
      _rayIndex++;
      _moveIndex = 0;
    }
    auto rayPointer = _currentListPointer->at(_rayIndex);
    auto move = rayPointer.at(_moveIndex);
    return move;
  }

  void skipRay() override {
    _rayIndex++;
    _moveIndex = 0;
  }

  // Prevent copying of the PieceMoves.
  PieceMovesNestedLists(const PieceMovesNestedLists &) = delete;
  PieceMovesNestedLists &operator=(const PieceMovesNestedLists &) = delete;

private:
  std::vector<std::vector<SquareIndex>> *_currentListPointer;
  int _rayIndex;
  int _moveIndex;

  // Private constructor to prevent creation of instances from outside the class.
  PieceMovesNestedLists() : _rayIndex(0), _moveIndex(0) {
    generateWhitePawnMoves();
    generateBlackPawnMoves();
    generateKnightMoves();
    generateBishopMoves();
    generateRookMoves();
    generateQueenMoves();
    generateKingMoves();
  };

  void generateWhitePawnMoves() {
    for (int index = 0; index <= 64; index++) {
      // Pawns cannot stand on 1st and 8th ranks
      if (index < 8 || index > 55) {
        continue;
      }

      int x = (int)(index % 8);
      int y = (int)((index / 8));

      // Diagonal kill to the left
      if (x > 0 && y > 0) {
        whitePawnMoves[index][0].push_back(index - 8 - 1);
      }
      // One Forward
      whitePawnMoves[index][1].push_back(index - 8);

      // Starting Position we can jump 2
      if (y == 6) {
        whitePawnMoves[index][1].push_back(index - 16);
      }
      // Diagonal Kill to the right
      if (x < 7 && y > 0) {
        whitePawnMoves[index][1].push_back(index - 8 + 1);
      }
    }
  }
  void generateBlackPawnMoves() {
    for (int index = 0; index <= 64; index++) {
      // Pawns cannot stand on 1st and 8th ranks
      if (index < 8 || index > 55) {
        continue;
      }

      int x = (int)(index % 8);
      int y = (int)((index / 8));

      // Diagonal kill to the left
      if (x > 0 && y > 0) {
        blackPawnMoves[index][0].push_back(index + 8 - 1);
      }
      // One Forward
      blackPawnMoves[index][1].push_back(index + 8);

      // Starting Position we can jump 2
      if (y == 6) {
        blackPawnMoves[index][1].push_back(index + 16);
      }
      // Diagonal Kill to the right
      if (x < 7 && y > 0) {
        blackPawnMoves[index][1].push_back(index + 8 + 1);
      }
    }
  }

  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generateKingMoves();
};

#endif

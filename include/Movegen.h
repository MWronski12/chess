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

class PieceValidMoves {
public:
  PieceValidMoves(PieceMovesInterface &movesIterator);
  PieceValidMoves &operator=(const PieceValidMoves &) { return *this; }
  PieceMovesInterface &movesIterator;
};
// Piece moves generator using nested vectors
// It is readable, but can loose some performance
class PieceMovesNestedLists : public PieceMovesInterface {
public:
  static PieceMovesNestedLists &getInstance() {
    static PieceMovesNestedLists instance;
    return instance;
  }

  static std::array<std::vector<std::vector<SquareIndex>>, 64> whitePawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> blackPawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> knightMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> bishopMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> rookMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> queenMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> kingMoves;

  // Iterator interface methods
  void start(PieceColor color, PieceType piece, SquareIndex src) override;
  bool hasNextMove() override;
  SquareIndex getNextMove() override;
  void skipRay() override;

  // Prevent copying of the PieceMoves.
  PieceMovesNestedLists(const PieceMovesNestedLists &) = delete;
  PieceMovesNestedLists &operator=(const PieceMovesNestedLists &) { return *this; }

private:
  std::vector<std::vector<SquareIndex>> *_currentListPointer;
  int _rayIndex;
  int _moveIndex;

  // Private constructor to prevent creation of instances from outside the class.
  PieceMovesNestedLists();

  void generateWhitePawnMoves();
  void generateBlackPawnMoves();
  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generateKingMoves();
};

#endif

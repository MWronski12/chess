#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "Piece.h
#include <array>
#include <vector>

#include <iostream>
#include <vector>

// class MyIterator {
// public:
//   MyIterator(std::vector<int> &data) : data_(data), index_(0) {}
//   bool hasNext() const { return index_ < data_.size(); }
//   int next() { return data_[index_++]; }

// private:
//   std::vector<int> &data_;
//   int index_;
// };

// int main() {
//   std::vector<int> numbers = {1, 2, 3, 4, 5};
//   MyIterator it(numbers);

//   // using the custom iterator to print out the values in the vector
//   while (it.hasNext()) {
//     std::cout << it.next() << " ";
//   }
//   std::cout << std::endl;

//   return 0;
// }

// piece generate valid moves korzysta z interfejsu PieceMovesInterface, który jest implementowany
// przez konkretną klasę np PieceMovesLists.

// void example() {
//   PieceMovesInterface::start(BISHOP, 10);
//   while (PieceMovesInterface::hasNext()) {
//     SquareIndex move = PieceMovesInterface::getNextMove();
//     if (true) { // empty square
//       continue;
//     } else if (false) { // piece encountered
//       PieceMovesInterface::skipRay();
//     }
//   }
// }

class PieceMovesInterface {
public:
  virtual void start(PieceColor color, PieceType piece, SquareIndex src) = 0;
  virtual bool hasNextMove() = 0;
  virtual SquareIndex getNextMove() = 0;
  virtual void skipRay() = 0;
};

class PieceMovesNestedLists : public PieceMovesInterface {
public:
  static std::array<std::vector<std::vector<SquareIndex>>, 64> whitePawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> blackPawnMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> knightMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> bishopMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> rookMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> queenMoves;
  static std::array<std::vector<std::vector<SquareIndex>>, 64> kingMoves;

  // Implement iterator methods
  void start(PieceColor color, PieceType piece, SquareIndex src) override {
    _squareIndex = src;
    switch (piece) {
    case /* constant-expression */:
      /* code */
      break;

    default:
      break;
    }
  }
  bool hasNextMove() override {}
  SquareIndex getNextMove() override {}
  void skipRay() override {
    _rayIndex++;
    _moveIndex = 0;
  }

  // Prevent copying of the PieceMoves.
  PieceMovesNestedLists(const PieceMovesNestedLists &) = delete;
  PieceMovesNestedLists &operator=(const PieceMovesNestedLists &) = delete;

private:
  std::array<std::vector<std::vector<SquareIndex>>, 64> *listPointer;
  SquareIndex _squareIndex;
  int _rayIndex;
  int _moveIndex;

  // Private constructor to prevent creation of instances from outside the class.
  PieceMovesNestedLists() : listPointer(NULL), _squareIndex(0), _rayIndex(0), _moveIndex(0) {
    generateWhitePawnMoves();
    generateBlackPawnMoves();
    generateKnightMoves();
    generateBishopMoves();
    generateRookMoves();
    generateQueenMoves();
    generateKingMoves();
  };

  void generateWhitePawnMoves();
  void generateBlackPawnMoves();
  void generateKnightMoves();
  void generateBishopMoves();
  void generateRookMoves();
  void generateQueenMoves();
  void generateKingMoves();
};

#endif

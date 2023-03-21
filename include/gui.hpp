#include "board.hpp"

#include <iostream>

class Gui {
public:
  virtual void makeMove(SquareIndex src, SquareIndex dest, PieceType promotion) = 0;
};

class ConsoleGui : public Gui {
public:
  ConsoleGui() : board(Board()) { draw(); }
  void makeMove(SquareIndex src, SquareIndex dest, PieceType promotion) override {
    board.makeMove(src, dest, promotion);
    draw();
  }

private:
  Board board;

  char pieceToChar(Piece piece) const {
    char result;
    switch (piece.getType()) {
    case KING:
      result = 'k';
      break;

    case QUEEN:
      result = 'q';
      break;

    case ROOK:
      result = 'r';
      break;

    case BISHOP:
      result = 'b';
      break;

    case KNIGHT:
      result = 'n';
      break;

    case PAWN:
      result = 'p';
      break;

    default:
      throw std::logic_error("Invalid piece type!");
    }

    // Capitalize for white piece
    if (piece.getColor() == WHITE) {
      result -= 20;
    }

    return result;
  }

  void draw() const {
    std::cout << std::endl;

    for (int i = 0; i < 64; i++) {
      if (i % 8 == 0) {
        std::cout << (64 - i) / 8 << " ";
      }
      std::cout << pieceToChar(*(board.squares[i]));
      if (i % 8 == 7) {
        std::cout << "\r\n";
      } else {
        std::cout << ", ";
      }
    }

    std::cout << "  A  B  C  D  E  F  G  H\r\n";
  }
};
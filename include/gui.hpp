#include "board.hpp"

#include <exception>
#include <iostream>

class Gui {
public:
  // By SquareIndex - upper left corner is 0 and bottom right corner is 63
  virtual void makeMove(SquareIndex src, SquareIndex dest, PieceType promotion) = 0;

  // By d2d4 notation
  virtual void makeMove(std::string move) = 0;
};

class ConsoleGui : public Gui {
public:
  ConsoleGui() : board(Board()) { draw(); }
  void makeMove(SquareIndex src, SquareIndex dest, PieceType promotion) override {
    board.makeMove(src, dest, promotion);
    draw();
  }

  void makeMove(std::string move) override {
    board.makeMove(move);
    draw();
  }

private:
  Board board;

  char pieceToChar(PieceColor color, PieceType type) const {
    char result;

    switch (type) {
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
    if (color == WHITE) {
      result = toupper(result);
    }

    return result;
  }

  void draw() const {
    std::cout << "\n\n";

    for (int i = 0; i < 64; i++) {
      if (i % 8 == 0) {
        std::cout << (64 - i) / 8 << " ";
      }
      if (board.squares[i]) {
        std::cout << pieceToChar(board.squares[i]->getColor(), board.squares[i]->type);
      } else {
        std::cout << '-';
      }
      if (i % 8 == 7) {
        std::cout << "\r\n";
      } else {
        std::cout << ", ";
      }
    }

    std::cout << "  A  B  C  D  E  F  G  H\r\n";
  }
};
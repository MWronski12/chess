#include "catch2/catch_test_macros.hpp"

#include "board.hpp"
#include <iostream>

/* ------------------------------ Constructors ------------------------------ */

TEST_CASE("Board constructs correct starting position by default", "[Board::Board()]") {
  Board board;
  for (int i = 0; i < 64; i++) {
    if (board.squares[i]) {
      REQUIRE(board.squares[i]->type == STARTING_POSITION[i]);
      if (i < 16) {
        REQUIRE(board.squares[i]->getColor() == BLACK);
      } else if (i > 47) {
        REQUIRE(board.squares[i]->getColor() == WHITE);
      }
    }
  }

  REQUIRE(board.whiteHasCastled() == false);
  REQUIRE(board.blackHasCastled() == false);
  REQUIRE(board.getEnPassantSquare() == NULL_SQUARE);
  REQUIRE(board.sideToMove == WHITE);
  REQUIRE(board.whiteIsChecked == false);
  REQUIRE(board.blackIsChecked == false);
  REQUIRE(board.whiteIsCheckMated == false);
  REQUIRE(board.blackIsCheckMated == false);
  REQUIRE(board.staleMate == false);
  REQUIRE(board.score == 0);
}

/* ----------------------------- Board::Validate ---------------------------- */

TEST_CASE("Cannot make move when the game is over!", "[Board::Validate()]") {
  Board board;
  board.staleMate = true;
  REQUIRE_THROWS_AS(board.makeMove(55, 47, EMPTY), std::logic_error);

  board = Board();
  board.blackIsCheckMated = true;
  REQUIRE_THROWS_AS(board.makeMove(55, 47, EMPTY), std::logic_error);

  board = Board();
  board.whiteIsCheckMated = true;
  REQUIRE_THROWS_AS(board.makeMove(55, 47, EMPTY), std::logic_error);
}

TEST_CASE("Invalid SquareIndices are not allowed!", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(-1, 64, EMPTY), std::invalid_argument);
}

TEST_CASE("Source square must be occupied by a piece", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(20, 28, EMPTY), std::logic_error);
}

TEST_CASE("Source square must be occupied by an allied piece", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(0, 18, EMPTY), std::logic_error);
}

TEST_CASE("Dest square cannot be occupied by an allied piece", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(0, 1, EMPTY), std::logic_error);
}

TEST_CASE("Promotion cannot be set to EMPTY for promoting moves", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(55, 7, EMPTY), std::logic_error);
}

TEST_CASE("Promotion must be set to EMPTY for non-promoting moves", "[Board::validateMove()]") {
  Board board;
  REQUIRE_THROWS_AS(board.makeMove(55, 47, QUEEN), std::logic_error);
}

/* ----------------------------- Board::makeMove ---------------------------- */

TEST_CASE("Move details are correctly recorded after making the move", "[Board::makeMove()]") {
  Board board;
  board.makeMove(55, 47, EMPTY);
  REQUIRE(board.lastMove.src == 55);
  REQUIRE(board.lastMove.dest == 47);
  REQUIRE(board.lastMove.isEnPassantCapture == false);
  REQUIRE(board.lastMove.pieceMoving == PAWN);
  REQUIRE(board.sideToMove == BLACK);

  board.makeMove(15, 47, EMPTY);
  REQUIRE(board.lastMove.pieceTaken == PAWN);
  REQUIRE(board.sideToMove == WHITE);

  board.makeMove(54, 6, QUEEN);
  REQUIRE(board.lastMove.promotion == QUEEN);
}

TEST_CASE("Board keeps track of en passant square", "[Board::makeMove()]") {
  Board board;
  board.makeMove(52, 36, EMPTY);
  REQUIRE(board.getEnPassantSquare() == 44);

  board.makeMove(12, 28, EMPTY);
  REQUIRE(board.getEnPassantSquare() == 20);

  board.makeMove(48, 40, EMPTY);
  REQUIRE(board.getEnPassantSquare() == NULL_SQUARE);
}

TEST_CASE("En passant capture is handled properly", "[Board::makeMove()]") {
  Board board;
  board.makeMove(52, 36, EMPTY);
  REQUIRE(board.getEnPassantSquare() == 44);
  board.makeMove(13, 44, EMPTY);

  if (board.squares[44].has_value())
    throw std::domain_error("ss");

  //   REQUIRE_FALSE(board.squares[44].has_value());
  //   REQUIRE(board.lastMove.isEnPassantCapture == true);

  //   if (board.squares[44].has_value()) {
  //     REQUIRE(true == false);
  //   }

  //   REQUIRE_FALSE(board.squares[44].has_value());

  //   REQUIRE(board.squares[36] == std::nullopt);
  //   board.makeMove(15, 31, EMPTY);
  //   board.makeMove(55, 23, EMPTY);
  //   REQUIRE(board.squares[31] == std::nullopt);
}

#include "catch2/catch_test_macros.hpp"

#include "piece.hpp"

TEST_CASE("Piece constructor assigns correct values", "[Piece::Piece]") {
  Piece piece = Piece(WHITE, PAWN, false);
  REQUIRE(piece.getColor() == WHITE);
  REQUIRE(piece.type == PAWN);
  REQUIRE(piece.hasMoved == false);
  REQUIRE(piece.getActionValue() == PAWN_ACTION_VALUE);
  REQUIRE(piece.getValue() == PAWN_VALUE);
  REQUIRE(piece.validMoves.empty());
}

#include "Piece.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Piece constructor assigns correct values", "[Piece::Piece]" ) {
    Piece piece = Piece( WHITE, PAWN, false );
    REQUIRE( piece.color == WHITE );
    REQUIRE( piece.type == PAWN );
    REQUIRE( piece.hasMoved == false );
    REQUIRE( piece.actionValue == PAWN_ACTION_VALUE );
    REQUIRE( piece.value == PAWN_VALUE );
    REQUIRE( piece.validMoves.empty() );
}

TEST_CASE( "Piece converts to string notation correctly", "[Piece::toString]" ) {
    Piece piece = Piece( WHITE, PAWN, false );
    REQUIRE( piece.toString() == "P" );
    piece = Piece( BLACK, PAWN, false );
    REQUIRE( piece.toString() == "p" );
    piece = Piece( WHITE, KNIGHT, false );
    REQUIRE( piece.toString() == "N" );
    piece = Piece( BLACK, KNIGHT, false );
    REQUIRE( piece.toString() == "n" );
    piece = Piece( WHITE, BISHOP, false );
    REQUIRE( piece.toString() == "B" );
    piece = Piece( BLACK, BISHOP, false );
    REQUIRE( piece.toString() == "b" );
    piece = Piece( WHITE, ROOK, false );
    REQUIRE( piece.toString() == "R" );
    piece = Piece( BLACK, ROOK, false );
    REQUIRE( piece.toString() == "r" );
    piece = Piece( WHITE, QUEEN, false );
    REQUIRE( piece.toString() == "Q" );
    piece = Piece( BLACK, QUEEN, false );
    REQUIRE( piece.toString() == "q" );
    piece = Piece( WHITE, KING, false );
    REQUIRE( piece.toString() == "K" );
    piece = Piece( BLACK, KING, false );
    REQUIRE( piece.toString() == "k" );
}

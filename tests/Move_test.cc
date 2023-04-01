#include "Move.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Move constructor assigns correct values", "[MoveContent::MoveContent]" ) {
    MoveContent move = MoveContent( 56, 49, EMPTY );
    REQUIRE( move.src == 56 );
    REQUIRE( move.dest == 49 );
    REQUIRE( move.promotion == EMPTY );
    REQUIRE( move.pieceMoving == EMPTY );
    REQUIRE( move.pieceTaken == EMPTY );
    REQUIRE( move.isEnPassantCapture == false );
    REQUIRE( move.score == 0 );
    REQUIRE( move.toPGN() == "O-O-O" );
}

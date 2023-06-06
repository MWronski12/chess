#include "MoveContent.h"

#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Move constructor assigns correct values", "[MoveContent::MoveContent]" ) {
    MoveContent move = MoveContent( A1, B2, EMPTY );
    REQUIRE( move.src == A1 );
    REQUIRE( move.dest == B2 );
    REQUIRE( move.promotion == EMPTY );
    REQUIRE( move.pieceMoving == EMPTY );
    REQUIRE( move.pieceTaken == EMPTY );
    REQUIRE( move.isEnPassantCapture == false );
    REQUIRE( move.score == 0 );
}

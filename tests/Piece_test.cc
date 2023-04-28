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

// TEST_CASE( "Piece copy contructor copies everything except for valid moves", "[Piece::Piece]" ) {
//     Piece original = Piece( WHITE, PAWN, false );
//     original.validMoves.push_back( 5 );

//     Piece piece( original );
//     REQUIRE( piece.color == WHITE );
//     REQUIRE( piece.type == PAWN );
//     REQUIRE( piece.hasMoved == false );
//     REQUIRE( piece.actionValue == PAWN_ACTION_VALUE );
//     REQUIRE( piece.value == PAWN_VALUE );
//     REQUIRE( piece.validMoves.empty() );
// }
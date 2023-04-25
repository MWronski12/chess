
#include "Search.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Evaluate moves returns array of 20 moves scored to 0 at initial position", "[Search.evaluateMoves]" ) {
    Search s;
    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto moves = s.evaluateMoves( b );

    REQUIRE( moves.size() == 20 );
    for ( auto move : moves ) {
        REQUIRE( move.score == 0 );
    }
}
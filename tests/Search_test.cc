
#include "Search.h"
#include "catch2/catch_test_macros.hpp"

// TEST_CASE( "Minimax at depth 1 returns score 0 at initial position", "[Search.miniMax]" ) {
//     Search s;
//     Board b;
//     PieceValidMoves g;
//     g.generateValidMoves( b );

//     REQUIRE( s.miniMax( 3, b ) == 0 );
// }

// TEST_CASE( "Evaluate moves returns array of 20 moves scored to 0 at initial position", "[Search.evaluateMoves]" ) {
//     Search s;
//     Board b;
//     PieceValidMoves g;
//     g.generateValidMoves( b );
//     auto moves = s.evaluateMoves( b );

//     REQUIRE( moves.size() == 20 );
//     for ( auto move : moves ) {
//         REQUIRE( move.score == 0 );
//     }
// }
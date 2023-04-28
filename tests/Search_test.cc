
#include "Search.h"
#include "catch2/catch_test_macros.hpp"

/* ------------------------------ evaluateMoves ----------------------------- */
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

/* ------------------------------- getBestMove ------------------------------ */
TEST_CASE( "Search finds the corect move for 1 ply fool's mate position for white", "[Search.getBestMove]" ) {
    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 2, b.sideToMove == WHITE );
    REQUIRE( bestMove.pieceMoving == QUEEN );
    REQUIRE( bestMove.src == 45 );
    REQUIRE( bestMove.dest == 13 );
}
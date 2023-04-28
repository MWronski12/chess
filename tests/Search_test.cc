#include <catch2/generators/catch_generators.hpp>

#include "Search.h"
#include "catch2/benchmark/catch_benchmark.hpp"
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
    auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );
    REQUIRE( bestMove.pieceMoving == QUEEN );
    REQUIRE( bestMove.src == 45 );
    REQUIRE( bestMove.dest == 13 );
}

TEST_CASE( "getBestMove multiple benchmarking", "[Search.getBestMove]" ) {
    int depth;
    MoveContent expected_result;
    std::tie( depth, expected_result ) = GENERATE( table<int, MoveContent>( {
        { 1, MoveContent( 45, 13 ) }, { 2, MoveContent( 45, 13 ) },
        // { 3, MoveContent( 45, 13 ) },
        // { 3, MoveContent(45, 13) },
    } ) );

    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );

    REQUIRE( s.getBestMove( b, depth, true ) == expected_result );
    BENCHMARK( "getBestMove search at depth " + std::to_string( depth ) ) { return s.getBestMove( b, depth, true ); };
}
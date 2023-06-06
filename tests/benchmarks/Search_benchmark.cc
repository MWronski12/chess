#include <catch2/generators/catch_generators.hpp>

#include "Search.h"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "getBestMove finds one move check mate instantly", "[Search.getBestMove]" ) {
    int depth;
    MoveContent expected_result;
    std::tie( depth, expected_result ) = GENERATE( table<int, MoveContent>( {
        { 1, MoveContent( F3, F7 ) },
        { 2, MoveContent( F3, F7 ) },
        { 3, MoveContent( F3, F7 ) },
        { 4, MoveContent( F3, F7 ) },
    } ) );

    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );

    REQUIRE( s.getBestMove( b, depth, true ) == expected_result );
    BENCHMARK( "getBestMove search at depth " + std::to_string( depth ) ) { return s.getBestMove( b, depth, true ); };
}
#include "Search.h"

#include <catch2/generators/catch_generators.hpp>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

// /* ------------------------------ evaluateMoves ----------------------------- */
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

// /* ------------------------------- getBestMove ------------------------------ */
// TEST_CASE( "Search finds the corect move for 1 ply fool's mate position for white", "[Search.getBestMove]" ) {
//     Search s;
//     Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
//     PieceValidMoves g;
//     g.generateValidMoves( b );
//     auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );
//     REQUIRE( bestMove.pieceMoving == QUEEN );
//     REQUIRE( bestMove.src == 45 );
//     REQUIRE( bestMove.dest == 13 );
// }

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

/* --------------------------------------------- testcases from internet -------------------------------------------- */

TEST_CASE( "Don't stalemate if you can win", "[search]" ) {
    // MagicMoves::initmagicmoves();
    // Tables::init();
    // ZK::initZobristKeys();
    // globalTT.init_TT_size( TT::TEST_MB_SIZE );  // For tests TT size is TT_SIZE_DEFAULT

    // std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "7k/8/7K/8/8/8/8/5R2 w - - 51 142" ) );
    // Search search( sp );
    // search.negaMaxRoot( 5 );
    // Move move = search.myBestMove;
    // REQUIRE( move.toShortString() == "f1f8" );

    Search s;
    Board b( "7k/8/7K/8/8/8/8/5R2 w - - 51 142" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );

    REQUIRE( bestMove.dest == 5 );
    REQUIRE( bestMove.src == 61 );
    BENCHMARK( "getBestMove search at depth 5 " ) { return s.getBestMove( b, 5, true ); };
}

TEST_CASE( "Search", "[search]" ) {
    // MagicMoves::initmagicmoves();
    // Tables::init();
    // ZK::initZobristKeys();
    // globalTT.init_TT_size( TT::TEST_MB_SIZE );

    SECTION( "Test search depth 1 white" ) {
        // std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "kn6/nn3r2/8/8/2p2Q2/8/NN6/KN6 w - -" ) );
        // Search search( sp );
        // search.negaMaxRoot( 1 );
        // Move move = search.myBestMove;

        // REQUIRE( move.toShortString() == "f4f7" );
        Search s;
        Board b( "kn6/nn3r2/8/8/2p2Q2/8/NN6/KN6 w - - 2 24" );
        PieceValidMoves g;
        g.generateValidMoves( b );
        auto bestMove = s.getBestMove( b, 1, b.sideToMove == WHITE );
        REQUIRE( bestMove.src == 37 );
        REQUIRE( bestMove.dest == 13 );
    }

    SECTION( "Test search depth 1 black" ) {
        // std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "k4K2/8/8/3q4/8/1R3N1P/8/8 b - -" ) );
        // Search search( sp );
        // search.negaMaxRoot( 1 );
        // Move move = search.myBestMove;

        // REQUIRE( move.toShortString() == "d5b3" );
        Search s;
        Board b( "k4K2/8/8/3q4/8/1R3N1P/8/8 b - - 2 24" );
        PieceValidMoves g;
        g.generateValidMoves( b );
        auto bestMove = s.getBestMove( b, 1, b.sideToMove == WHITE );
        REQUIRE( bestMove.src == 27 );
        REQUIRE( bestMove.dest == 41 );
    }

    SECTION( "Test search depth 2" ) {
        // std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "kn6/nn2rr2/8/4Q3/8/2p1p2b/1Q6/KN6 w - -" ) );
        // Search search( sp );
        // search.negaMaxRoot( 2 );
        // Move move = search.myBestMove;
        // REQUIRE( move.toShortString() == "e5c3" );
        Search s;
        Board b( "kn6/nn2rr2/8/4Q3/8/2p1p2b/1Q6/KN6 w - - 2 24" );
        PieceValidMoves g;
        g.generateValidMoves( b );
        auto bestMove = s.getBestMove( b, 1, b.sideToMove == WHITE );
        REQUIRE( bestMove.src == 28 );
        REQUIRE( bestMove.dest == 42 );
    }

    SECTION( "Always find the quickest mate" ) {
        // std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "8/3R4/3p4/3P4/1K4Q1/8/7k/8 w - - 97 104" ) );
        // Search search( sp );

        // for ( int i = 2; i < 7; i++ ) {
        //     search.negaMaxRoot( i );
        //     Move move = search.myBestMove;
        //     REQUIRE( move.toShortString() == "d7h7" );
        // }
        Search s;
        Board b( "8/3R4/3p4/3P4/1K4Q1/8/7k/8 w - - 97 104" );
        PieceValidMoves g;
        g.generateValidMoves( b );

        for ( int i = 2; i < 7; i++ ) {
            auto bestMove = s.getBestMove( b, i, b.sideToMove == WHITE );
            REQUIRE( bestMove.src == 11 );
            REQUIRE( bestMove.dest == 15 );
            // BENCHMARK( "getBestMove search at depth " + std::to_string( i ) ) { return s.getBestMove( b, i, true ); };
        }
    }
}
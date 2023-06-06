#include "Search.h"

#include "catch2/catch_test_macros.hpp"

/* ------------------------------ evaluateMoves ----------------------------- */
TEST_CASE( "Evaluate moves returns array of 20 moves scored to 0 at initial position", "[Search.evaluateMoves]" ) {
    Search s;
    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto moves = s.getPossibleMoves( b );

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

/* ----------------------------- quiescentSearch ---------------------------- */

TEST_CASE( "getPossibleCaptureMoves returns correctly sorted capture moves ", "[Search.getPossibleCaptureMoves]" ) {
    Search s;
    Board b( "4n3/3bQp2/3qr3/8/8/8/8/k1K5 w - - 0 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto moves = s.getPossibleCaptureMoves( b );
    REQUIRE( moves.size() == 5 );             // 5 captures
    REQUIRE( moves[0].pieceTaken == QUEEN );  // Highest value attacked first
    REQUIRE( moves[1].pieceTaken == ROOK );
    REQUIRE( moves[2].pieceTaken == BISHOP );
    REQUIRE( moves[3].pieceTaken == KNIGHT );
    REQUIRE( moves[4].pieceTaken == PAWN );
}

TEST_CASE( "quiescentSearch returns evaluation of a quiet position ", "[Search.quiescentSearch]" ) {
    Search s;
    Board b( "k7/8/8/8/8/8/6K1/6n1 w - - 0 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    int nodesExamined = 0, nodesEvaluated = 0, nodesPruned = 0;
    int quietEval = s.quiescentSearch( b, 5, NEGATIVE_INFINITY, POSITIVE_INFINITY, true, nodesExamined, nodesEvaluated,
                                       nodesPruned );
    REQUIRE( quietEval == 10 );
    REQUIRE( nodesExamined == 2 );
    REQUIRE( nodesEvaluated == 1 );
    REQUIRE( nodesPruned == 0 );
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

    REQUIRE( bestMove.src == 61 );
    REQUIRE( bestMove.dest == 5 );
}

// TEST_CASE( "Search", "[search]" ) {
//     MagicMoves::initmagicmoves();
//     Tables::init();
//     ZK::initZobristKeys();
//     globalTT.init_TT_size( TT::TEST_MB_SIZE );

//     SECTION( "Test search depth 1 white" ) {
//         std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "kn6/nn3r2/8/8/2p2Q2/8/NN6/KN6 w - -" ) );
//         Search search( sp );
//         search.negaMaxRoot( 1 );
//         Move move = search.myBestMove;

//         REQUIRE( move.toShortString() == "f4f7" );
//     }

//     SECTION( "Test search depth 1 black" ) {
//         std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "k4K2/8/8/3q4/8/1R3N1P/8/8 b - -" ) );
//         Search search( sp );
//         search.negaMaxRoot( 1 );
//         Move move = search.myBestMove;

//         REQUIRE( move.toShortString() == "d5b3" );
//     }

//     SECTION( "Test search depth 2" ) {
//         std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "kn6/nn2rr2/8/4Q3/8/2p1p2b/1Q6/KN6 w - -" ) );
//         Search search( sp );
//         search.negaMaxRoot( 2 );
//         Move move = search.myBestMove;

//         REQUIRE( move.toShortString() == "e5c3" );
//     }

//     SECTION( "Always find the quickest mate" ) {
//         std::shared_ptr<Board> sp = std::shared_ptr<Board>( new Board( "8/3R4/3p4/3P4/1K4Q1/8/7k/8 w - - 97 104" ) );
//         Search search( sp );

//         for ( int i = 2; i < 7; i++ ) {
//             search.negaMaxRoot( i );
//             Move move = search.myBestMove;
//             REQUIRE( move.toShortString() == "d7h7" );
//         }
//     }
// }
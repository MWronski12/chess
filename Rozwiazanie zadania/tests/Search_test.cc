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
    Search s;
    Board b( "7k/8/7K/8/8/8/8/5R2 w - - 51 142" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );

    REQUIRE( bestMove.src == 61 );
    REQUIRE( bestMove.dest == 5 );
}

TEST_CASE( "Fool's mate position for white", "[Search.getBestMove]" ) {
    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );
    REQUIRE( bestMove.pieceMoving == QUEEN );
    REQUIRE( bestMove.src == 45 );
    REQUIRE( bestMove.dest == 13 );
}

TEST_CASE( "Mate in two - Paul Morphy's problem", "[Search.getBestMove]" ) {
    Search s;
    Board b( "kbK5/pp6/1P6/8/8/8/8/R7 w - - 1 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 10, b.sideToMove == WHITE );

    REQUIRE( bestMove.src == A1 );
    REQUIRE( bestMove.dest == A6 );
}

TEST_CASE( "Mate in two with 2 queens", "[Search.getBestMove]" ) {
    Search s;
    Board b( "Q7/7k/Q7/8/8/8/7K/1q6 w - - 0 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 10, b.sideToMove == WHITE );

    REQUIRE( bestMove.src == A8 );
    REQUIRE( bestMove.dest == A7 );
}

TEST_CASE( "Mate in two - Milan Vidmar vs. Max Euwe", "[Search.getBestMove]" ) {
    Search s;
    Board b( "6k1/5p2/1p5p/p4Np1/5q2/Q6P/PPr5/3R3K w - - 1 0" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 10, b.sideToMove == WHITE );

    REQUIRE( bestMove.src == A3 );
    REQUIRE( bestMove.dest == F8 );
}

#include <stdint.h>

#include <catch2/generators/catch_generators.hpp>

#include "Engine.h"
#include "Movegen.h"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

/* ---------------------------------- Perft --------------------------------- */

uint64_t perft( int depth, Board &board, PieceValidMoves &generator ) {
    if ( depth == 0 ) {
        return 1;
    }

    uint64_t nodes = 0;

    // Cache the board
    Board currentBoard = Board( board );

    // Iterate through every piece on the board
    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = board.squares[srcSquare];

        if ( piece == std::nullopt || piece->color != board.sideToMove ) {
            continue;
        }

        // Iterate through every pseudo valid move for the piece
        for ( auto destSquare : piece->validMoves ) {
            // For pawn promotions
            if ( piece->type == PAWN && ( destSquare < 8 || destSquare > 55 ) ) {
                for ( PieceType promotionType : { KNIGHT, BISHOP, ROOK, QUEEN } ) {
                    // Make the move
                    board.makeMove( srcSquare, destSquare, promotionType );
                    generator.generateValidMoves( board );

                    // Add subnodes count if the move is valid
                    if ( generator.validateBoard( board ) == true ) {
                        nodes += perft( depth - 1, board, generator );
                    }

                    // Undo the move
                    board = currentBoard;
                }
                continue;
            }
            // For rest of the moves
            else {
                // Make the move
                board.makeMove( srcSquare, destSquare );
                generator.generateValidMoves( board );

                // Add subnodes count if the move is valid
                if ( generator.validateBoard( board ) ) {
                    nodes += perft( depth - 1, board, generator );
                }

                // Undo the move
                board = currentBoard;
            }
        }
    }

    return nodes;
}

/* -------------------- benchmarks from starting position ------------------- */

// TEST_CASE( "Perft function multiple benchmarking", "[perft]" ) {
//     int depth;
//     uint64_t expected_result;
//     std::tie( depth, expected_result ) = GENERATE( table<int, uint64_t>( {
//         { 0, 1 },
//         // { 1, 20 },
//         // { 2, 400 },
//         // { 3, 8'902 },
//         // { 4, 197'281 },
//     } ) );

//     Board b;
//     PieceValidMoves g;
//     g.generateValidMoves( b );
//     REQUIRE( perft( depth, b, g ) == expected_result );
//     BENCHMARK( "Perft at depth " + std::to_string( depth ) ) { return perft( depth, b, g ); };
// }

/* --------------------- different fen loaded positions --------------------- */

// https://www.chessprogramming.org/Perft_Results

TEST_CASE( "Fen starting position 1", "[perft]" ) {
    Board b( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 20 );
    REQUIRE( perft( 2, b, g ) == 400 );
    REQUIRE( perft( 3, b, g ) == 8902 );
    // REQUIRE( perft( 4, b, g ) == 197281 );
}

TEST_CASE( "Fen position 2", "[perft]" ) {
    Board b( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 48 );
    REQUIRE( perft( 2, b, g ) == 2039 );
    REQUIRE( perft( 3, b, g ) == 97862 );
    // REQUIRE( perft( 4, b, g ) == 4085603 );
    // REQUIRE( perft( 5, b, g ) == 193690690 );
}

TEST_CASE( "Fen position 3", "[perft]" ) {
    Board b( "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 14 );
    REQUIRE( perft( 2, b, g ) == 191 );
    REQUIRE( perft( 3, b, g ) == 2812 );
    REQUIRE( perft( 4, b, g ) == 43238 );
    // REQUIRE( perft( 5, b, g ) == 674624 );
}

TEST_CASE( "Fen position 4", "[perft]" ) {
    Board b( "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 6 );
    REQUIRE( perft( 2, b, g ) == 264 );
    REQUIRE( perft( 3, b, g ) == 9467 );
    REQUIRE( perft( 4, b, g ) == 422333 );
    // REQUIRE( perft( 5, b, g ) == 15833292 );
    // REQUIRE( perft( 6, b, g ) == 706045033 );
}

TEST_CASE( "Fen position 5", "[perft]" ) {
    Board b( "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 44 );
    REQUIRE( perft( 2, b, g ) == 1486 );
    REQUIRE( perft( 3, b, g ) == 62379 );
    // REQUIRE( perft( 4, b, g ) == 2103487 );
    // REQUIRE( perft( 5, b, g ) == 89941194 );
}

TEST_CASE( "Fen position 6", "[perft]" ) {
    Board b( "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 1, b, g ) == 46 );
    REQUIRE( perft( 2, b, g ) == 2079 );
    REQUIRE( perft( 3, b, g ) == 89890 );
    // REQUIRE( perft( 4, b, g ) == 3894594 );
    // REQUIRE( perft( 5, b, g ) == 164075551 );
}

#include <stdint.h>

#include <catch2/generators/catch_generators.hpp>

#include "Engine.h"
#include "Movegen.h"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

/* -------------------------------------------------------------------------- */
/*                              Piece Valid Moves                             */
/* -------------------------------------------------------------------------- */

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

        if ( piece == std::nullopt || piece->getColor() != board.sideToMove ) {
            continue;
        }

        // Iterate through every pseudo valid move for the piece
        for ( auto destSquare : piece->validMoves ) {
            // Make the move
            board.makeMove( srcSquare, destSquare );
            generator.generateValidMoves( board );

            // Add subnodes count if the move is valid
            if ( !( board.blackIsChecked && board.sideToMove == WHITE ) &&
                 !( board.whiteIsChecked && board.sideToMove == BLACK ) ) {
                nodes += perft( depth - 1, board, generator );
            }

            // Undo the move
            board = currentBoard;
        }
    }

    return nodes;
}

TEST_CASE( "Perft function multiple benchmarking", "[perft]" ) {
    int depth;
    uint64_t expected_result;
    std::tie( depth, expected_result ) = GENERATE( table<int, uint64_t>( {
        { 0, 1 }, { 1, 20 }, { 2, 400 },
        // { 3, 8'902 },
        // { 4, 197'281 },
    } ) );

    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( depth, b, g ) == expected_result );
    BENCHMARK( "Perft at depth " + std::to_string( depth ) ) { return perft( depth, b, g ); };
}

TEST_CASE( "Perft function single test", "[perft]" ) {
    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( 5, b, g ) == 4'865'609 );
}

#include <catch2/generators/catch_generators.hpp>

#include "Movegen.h"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

uint64_t perft( int depth, Board &board, PieceValidMoves &generator ) {
    if ( depth == 0 ) {
        return 1;
    }

    uint64_t nodes = 0;

    // Cache the board
    Board currentBoard( board );

    // Iterate through every piece on the board
    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        const auto piece = board.squares[srcSquare];

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
                if ( generator.validateBoard( board ) == true ) {
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

TEST_CASE( "Perft function multiple benchmarking", "[perft]" ) {
    int depth;
    uint64_t expected_result;
    std::tie( depth, expected_result ) = GENERATE( table<int, uint64_t>( {
        { 0, 1 }, { 1, 20 }, { 2, 400 }, { 3, 8'902 },
        // { 4, 197'281 },
    } ) );

    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    REQUIRE( perft( depth, b, g ) == expected_result );
    BENCHMARK( "Perft at depth " + std::to_string( depth ) ) { return perft( depth, b, g ); };
}
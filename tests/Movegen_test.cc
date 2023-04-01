#include <stdint.h>

#include <catch2/generators/catch_generators.hpp>

#include "Engine.h"
#include "Movegen.h"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"
/* -------------------------------------------------------------------------- */
/*                              Piece Valid Moves                             */
/* -------------------------------------------------------------------------- */

uint64_t perft( int depth, Engine &e ) {
    uint64_t nodes = 0;

    if ( depth == 0 ) {
        return 1;
    }

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = e.board->squares[srcSquare];

        if ( piece == std::nullopt || piece->getColor() != e.whooseMove() ) {
            continue;
        }

        for ( auto destSquare : piece->validMoves ) {
            std::unique_ptr<Board> root = std::make_unique<Board>( *e.board );
            if ( e.makeMove( srcSquare, destSquare ) == true ) {
                nodes += perft( depth - 1, e );
                e.board = std::move( root );
            }
        }
    }

    return nodes;
}

// TEST_CASE( "Perft function validation and benchmark", "[perft]" ) {
//     Engine e;
//     REQUIRE( perft( 2, e ) == 400 );
//     REQUIRE( perft( 3, e ) == 8902 );
//     REQUIRE( perft( 4, e ) == 197281 );
// }

TEST_CASE( "Perft function validation and benchmark", "[perft]" ) {
    Engine e;

    int depth;
    uint64_t expected_result;
    std::tie( depth, expected_result ) = GENERATE( table<int, uint64_t>( {
        { 0, 1 },
        { 1, 20 },
        { 2, 400 },
    } ) );

    REQUIRE( perft( depth, e ) == expected_result );

    BENCHMARK( "Perft at depth " + std::to_string( depth ) ) { return perft( depth, e ); };
}

/* -------------------------------------------------------------------------- */
/*                                 Piece Moves                                */
/* -------------------------------------------------------------------------- */

// I took the data from this post:
// https://www.chess.com/blog/the_real_greco/another-silly-question-how-many-chess-moves-are-there

static int countTotalMoves( PieceColor color, PieceType type ) {
    PieceMoves &pieceMoves = PieceMoves::getInstance();
    int count = 0;

    for ( SquareIndex square = 0; square < 64; square++ )
        for ( auto &ray : pieceMoves.getMoveList( color, type, square ) ) {
            for ( auto &_ : ray ) {
                (void)_;
                count++;
            }
        }

    return count;
}

/* ---------------------------------- PAWNS --------------------------------- */

TEST_CASE( "Number of total white pawn moves is 140", "[PieceMoves._whitePawnMoves]" ) {
    // 6 ranks * (6 files * 3 moves per rank + 2 files * 2 moves per rank) + 8
    // double advance moves
    REQUIRE( countTotalMoves( WHITE, PAWN ) == 6 * ( 6 * 3 + 2 * 2 ) + 8 );  // 140 moves total
}

TEST_CASE( "Number of total black pawn moves is 140", "[PieceMoves._blackPawnMoves]" ) {
    // 6 ranks * (6 files * 3 moves per rank + 2 files * 2 moves per rank) + 8
    // double advance moves
    REQUIRE( countTotalMoves( BLACK, PAWN ) == 6 * ( 6 * 3 + 2 * 2 ) + 8 );  // 140 moves total
}

/* --------------------------------- KNIGHTS -------------------------------- */

TEST_CASE( "Number of total knight moves is 336", "[PieceMoves._knightMoves]" ) {
    REQUIRE( countTotalMoves( WHITE, KNIGHT ) == 336 );
}

/* --------------------------------- BISHOPS -------------------------------- */

TEST_CASE( "Number of total knight moves is 560", "[PieceMoves._bishopMoves]" ) {
    REQUIRE( countTotalMoves( WHITE, BISHOP ) == 560 );
}

/* ---------------------------------- ROOKS --------------------------------- */

TEST_CASE( "Number of total knight moves is 896", "[PieceMoves._rookMoves]" ) {
    REQUIRE( countTotalMoves( WHITE, ROOK ) == 896 );
}

/* ---------------------------------- QUEEN --------------------------------- */

TEST_CASE( "Number of total queen moves is 1456", "[PieceMoves._queenMoves]" ) {
    REQUIRE( countTotalMoves( WHITE, QUEEN ) == 1456 );
}

/* ----------------------------------- KING --------------------------------- */
TEST_CASE( "Number of total king moves is 424", "[PieceMoves._kingMoves]" ) {
    REQUIRE( countTotalMoves( WHITE, KING ) == 424 );
}

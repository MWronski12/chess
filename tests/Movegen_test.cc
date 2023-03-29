#include <stdint.h>

#include "Movegen.h"
#include "catch2/catch_test_macros.hpp"

/* -------------------------------------------------------------------------- */
/*                              Piece Valid Moves                             */
/* -------------------------------------------------------------------------- */

// TODO Shouldn't be coupled!
#include "Engine.h"
uint64_t perft( uint8_t depth, Engine& engine, PieceValidMoves& moveGenerator ) {
    if ( depth == 0 ) {
        return 1;
    }
    uint64_t nodes = 0;

    moveGenerator.generateValidMoves( engine.board );

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = engine.board.squares[srcSquare];
        if ( !piece || piece->getColor() != engine.board.sideToMove ) {
            continue;
        }

        for ( auto& destSquare : piece->validMoves ) {
            Board prevBoard = engine.board;

            if ( engine.makeMove( srcSquare, destSquare ) ) {
                nodes += perft( depth - 1, engine, moveGenerator );
            }
            engine.board = prevBoard;
        }
    }

    return nodes;
}

TEST_CASE( "All possible moves count at depth 1 is 20", "[perft]" ) {
    Engine e;
    PieceValidMoves g;
    REQUIRE( perft( 1, e, g ) == 20 );
}

TEST_CASE( "All possible moves count at depth 2 is 400", "[perft]" ) {
    Engine e;
    PieceValidMoves g;
    REQUIRE( perft( 2, e, g ) == 400 );
}

TEST_CASE( "All possible moves count at depth 3 is 8902", "[perft]" ) {
    Engine e;
    PieceValidMoves g;
    REQUIRE( perft( 3, e, g ) == 8902 );
}

/* -------------------------------------------------------------------------- */
/*                                 Piece Moves                                */
/* -------------------------------------------------------------------------- */

// I took the data from this post:
// https://www.chess.com/blog/the_real_greco/another-silly-question-how-many-chess-moves-are-there

static int countTotalMoves( PieceColor color, PieceType type ) {
    PieceMoves& pieceMoves = PieceMoves::getInstance();
    int count = 0;

    for ( SquareIndex square = 0; square < 64; square++ )
        for ( auto& ray : pieceMoves.getMoveList( color, type, square ) ) {
            for ( auto& _ : ray ) {
                (void)_;
                count++;
            }
        }

    return count;
}

/* ---------------------------------- PAWNS --------------------------------- */

TEST_CASE( "Number of total white pawn moves is 140", "[PieceMoves._whitePawnMoves]" ) {
    // 6 ranks * (6 files * 3 moves per rank + 2 files * 2 moves per rank) + 8 double advance moves
    REQUIRE( countTotalMoves( WHITE, PAWN ) == 6 * ( 6 * 3 + 2 * 2 ) + 8 );  // 140 moves total
}

TEST_CASE( "Number of total black pawn moves is 140", "[PieceMoves._blackPawnMoves]" ) {
    // 6 ranks * (6 files * 3 moves per rank + 2 files * 2 moves per rank) + 8 double advance moves
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

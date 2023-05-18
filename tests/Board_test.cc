#include <iostream>

#include "Board.h"
#include "catch2/catch_test_macros.hpp"

/* ------------------------------- Constructor ------------------------------ */
TEST_CASE( "Board constructs correct starting position by default", "[Board::Board()]" ) {
    Board board;
    for ( int i = 0; i < 64; i++ ) {
        if ( board.squares[i] ) {
            REQUIRE( board.squares[i]->type == STARTING_POSITION[i] );
            if ( i < 16 ) {
                REQUIRE( board.squares[i]->color == BLACK );
            } else if ( i > 47 ) {
                REQUIRE( board.squares[i]->color == WHITE );
            }
        }
    }

    REQUIRE( board.whiteHasCastled == false );
    REQUIRE( board.blackHasCastled == false );
    REQUIRE( board.enPassantSquare == NULL_SQUARE );
    REQUIRE( board.sideToMove == WHITE );
    REQUIRE( board.whiteIsChecked == false );
    REQUIRE( board.gameResult == IN_PROGRESS );
    REQUIRE( board.score == 0 );
}

/* ----------------------------- Board::Validate ---------------------------- */

TEST_CASE( "Cannot make move when the game is over!", "[Board::Validate()]" ) {
    Board board;
    board.gameResult = DRAW;
    REQUIRE_THROWS_AS( board.makeMove( 55, 47, EMPTY ), std::logic_error );

    board = Board();
    board.gameResult = WHITE_WON;
    REQUIRE_THROWS_AS( board.makeMove( 55, 47, EMPTY ), std::logic_error );

    board = Board();
    board.gameResult = BLACK_WON;
    REQUIRE_THROWS_AS( board.makeMove( 55, 47, EMPTY ), std::logic_error );
}

TEST_CASE( "Invalid SquareIndices are not allowed!", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( -1, 64, EMPTY ), std::invalid_argument );
}

TEST_CASE( "Source square must be occupied by a piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( 20, 28, EMPTY ), std::logic_error );
}

TEST_CASE( "Source square must be occupied by an allied piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( 0, 18, EMPTY ), std::logic_error );
}

TEST_CASE( "Dest square cannot be occupied by an allied piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( 0, 1, EMPTY ), std::logic_error );
}

TEST_CASE( "Promotion cannot be set to EMPTY for promoting moves", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( 55, 7, EMPTY ), std::logic_error );
}

TEST_CASE( "Promotion must be set to EMPTY for non-promoting moves", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( 55, 47, QUEEN ), std::logic_error );
}

/* ----------------------------- Board::makeMove ---------------------------- */

TEST_CASE( "Move details are correctly recorded after making the move", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( 55, 47, EMPTY );
    REQUIRE( board.lastMove.src == 55 );
    REQUIRE( board.lastMove.dest == 47 );
    REQUIRE( board.lastMove.isEnPassantCapture == false );
    REQUIRE( board.lastMove.pieceMoving == PAWN );
    REQUIRE( board.sideToMove == BLACK );

    board.makeMove( 15, 47, EMPTY );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
    REQUIRE( board.sideToMove == WHITE );

    board.makeMove( 54, 6, QUEEN );
    REQUIRE( board.lastMove.promotion == QUEEN );
}

TEST_CASE( "Board keeps track of en passant square", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( 52, 36, EMPTY );
    REQUIRE( board.enPassantSquare == 44 );

    board.makeMove( 12, 28, EMPTY );
    REQUIRE( board.enPassantSquare == 20 );

    board.makeMove( 48, 40, EMPTY );
    REQUIRE( board.enPassantSquare == NULL_SQUARE );
}

TEST_CASE( "En passant capture is handled properly", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( 52, 36, EMPTY );
    REQUIRE( board.enPassantSquare == 44 );
    board.makeMove( 13, 44, EMPTY );
    REQUIRE( board.squares[36] == std::nullopt );
    REQUIRE( board.lastMove.isEnPassantCapture == true );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
    board.makeMove( 55, 47, EMPTY );
    board.makeMove( 11, 27, EMPTY );
    board.makeMove( 50, 19, EMPTY );
    REQUIRE( board.squares[27] == std::nullopt );
    REQUIRE( board.lastMove.isEnPassantCapture == true );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
}

TEST_CASE( "Castle king side is handled properly", "Board::makeMove()" ) {
    Board board;
    // move away the horses - dest square cannot be occupied by an allied piece
    board.makeMove( 62, 45, EMPTY );
    board.makeMove( 6, 21, EMPTY );
    // white castle king side
    board.makeMove( 60, 62, EMPTY );
    REQUIRE( board.squares[63] == std::nullopt );
    REQUIRE( board.squares[61]->type == ROOK );
    REQUIRE( board.squares[61]->hasMoved == true );
    REQUIRE( board.whiteHasCastled == true );
    // black castle king side
    board.makeMove( 4, 6, EMPTY );
    REQUIRE( board.squares[7] == std::nullopt );
    REQUIRE( board.squares[5]->type == ROOK );
    REQUIRE( board.squares[5]->hasMoved == true );
    REQUIRE( board.blackHasCastled == true );
}

TEST_CASE( "Castle queen side is handled properly", "Board::makeMove()" ) {
    Board board;
    // move away the bishops - dest square cannot be occupied by an allied piece
    board.makeMove( 58, 44, EMPTY );
    board.makeMove( 2, 20, EMPTY );
    // white castle queen side
    board.makeMove( 60, 58, EMPTY );
    REQUIRE( board.squares[56] == std::nullopt );
    REQUIRE( board.squares[59]->type == ROOK );
    REQUIRE( board.squares[59]->hasMoved == true );
    REQUIRE( board.whiteHasCastled == true );
    // black castle king side
    board.makeMove( 4, 2, EMPTY );
    REQUIRE( board.squares[0] == std::nullopt );
    REQUIRE( board.squares[3]->type == ROOK );
    REQUIRE( board.squares[3]->hasMoved == true );
    REQUIRE( board.blackHasCastled == true );
}

TEST_CASE( "Promotion is handled correctly", "Board::makeMove()" ) {
    Board board;
    board.makeMove( 55, 7, KNIGHT );
    REQUIRE( board.squares[7]->type == KNIGHT );
    REQUIRE( board.squares[7]->value == KNIGHT_VALUE );
    REQUIRE( board.squares[7]->actionValue == KNIGHT_ACTION_VALUE );
}

TEST_CASE( "Pieces are moved correctly", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( 53, 37, EMPTY );
    REQUIRE( board.squares[53] == std::nullopt );
    REQUIRE( board.squares[37]->type == PAWN );

    board.makeMove( 6, 21, EMPTY );
    REQUIRE( board.squares[6] == std::nullopt );
    REQUIRE( board.squares[21]->type == KNIGHT );
}

/* ---------------------------- FEN serialization --------------------------- */
TEST_CASE( "Board serializes and deserializes properly to fen", "Board::Board(std::string fen) Board::toFEN()" ) {
    Board board;
    std::string fen = board.toFEN();
    Board board2( fen );
    REQUIRE( board2.toFEN() == fen );

    // Piece placement
    for ( SquareIndex i = 0; i < 64; i++ ) {
        auto piece = board.squares[i];
        auto piece2 = board2.squares[i];
        if ( piece ) {
            REQUIRE( piece2 );
            REQUIRE( piece->type == piece2->type );
            REQUIRE( piece->color == piece2->color );
        } else {
            REQUIRE( !piece2 );
        }
    }

    // Side to move
    REQUIRE( board2.sideToMove == board.sideToMove );

    // Enpassant
    REQUIRE( board2.enPassantSquare == board.enPassantSquare );

    std::string fen2 = board2.toFEN();
    REQUIRE( fen2 == fen );
}
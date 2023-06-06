#include "Board.h"

#include <iostream>

#include "catch2/catch_test_macros.hpp"

/* ------------------------------- Constructor ------------------------------ */
TEST_CASE( "Board constructs correct starting position by default", "[Board::Board()]" ) {
    Board board;
    for ( int i = 0; i < 64; i++ ) {
        if ( board.squares[i] ) {
            REQUIRE( board.squares[i]->type == STARTING_POSITION[i] );
            if ( i < A6 ) {
                REQUIRE( board.squares[i]->color == BLACK );
            } else if ( i > H3 ) {
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
    REQUIRE_THROWS_AS( board.makeMove( H2, H3, EMPTY ), std::logic_error );

    board = Board();
    board.gameResult = WHITE_WON;
    REQUIRE_THROWS_AS( board.makeMove( H2, H3, EMPTY ), std::logic_error );

    board = Board();
    board.gameResult = BLACK_WON;
    REQUIRE_THROWS_AS( board.makeMove( H2, H3, EMPTY ), std::logic_error );
}

TEST_CASE( "Invalid SquareIndices are not allowed!", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( -1, 64, EMPTY ), std::invalid_argument );
}

TEST_CASE( "Source square must be occupied by a piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( E6, E5, EMPTY ), std::logic_error );
}

TEST_CASE( "Source square must be occupied by an allied piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( A7, A6, EMPTY ), std::logic_error );
}

TEST_CASE( "Dest square cannot be occupied by an allied piece", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( G2, H2, EMPTY ), std::logic_error );
}

TEST_CASE( "Promotion cannot be set to EMPTY for promoting moves", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( H2, H8, EMPTY ), std::logic_error );
}

TEST_CASE( "Promotion must be set to EMPTY for non-promoting moves", "[Board::validateMove()]" ) {
    Board board;
    REQUIRE_THROWS_AS( board.makeMove( H2, H3, QUEEN ), std::logic_error );
}

/* ----------------------------- Board::makeMove ---------------------------- */

TEST_CASE( "Move details are correctly recorded after making the move", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( H2, H3, EMPTY );
    REQUIRE( board.lastMove.src == H2 );
    REQUIRE( board.lastMove.dest == H3 );
    REQUIRE( board.lastMove.isEnPassantCapture == false );
    REQUIRE( board.lastMove.pieceMoving == PAWN );
    REQUIRE( board.sideToMove == BLACK );

    board.makeMove( H7, H3, EMPTY );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
    REQUIRE( board.sideToMove == WHITE );

    board.makeMove( G2, G8, QUEEN );
    REQUIRE( board.lastMove.promotion == QUEEN );
}

TEST_CASE( "Board keeps track of en passant square", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( E2, E4, EMPTY );
    REQUIRE( board.enPassantSquare == E3 );

    board.makeMove( E7, E5, EMPTY );
    REQUIRE( board.enPassantSquare == E6 );

    board.makeMove( A2, A3, EMPTY );
    REQUIRE( board.enPassantSquare == NULL_SQUARE );
}

TEST_CASE( "En passant capture is handled properly", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( E2, E4, EMPTY );
    REQUIRE( board.enPassantSquare == E3 );
    board.makeMove( F7, E3, EMPTY );
    REQUIRE( board.squares[E4] == std::nullopt );
    REQUIRE( board.lastMove.isEnPassantCapture == true );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
    board.makeMove( H2, H3, EMPTY );
    board.makeMove( D7, D5, EMPTY );
    board.makeMove( C2, D6, EMPTY );
    REQUIRE( board.squares[D5] == std::nullopt );
    REQUIRE( board.lastMove.isEnPassantCapture == true );
    REQUIRE( board.lastMove.pieceTaken == PAWN );
}

TEST_CASE( "Castle king side is handled properly", "Board::makeMove()" ) {
    Board board;
    // move away the horses - dest square cannot be occupied by an allied piece
    board.makeMove( G1, F3, EMPTY );
    board.makeMove( G8, F6, EMPTY );
    // white castle king side
    board.makeMove( E1, G1, EMPTY );
    REQUIRE( board.squares[H1] == std::nullopt );
    REQUIRE( board.squares[F1]->type == ROOK );
    REQUIRE( board.squares[F1]->hasMoved == true );
    REQUIRE( board.whiteHasCastled == true );
    // black castle king side
    board.makeMove( E8, G8, EMPTY );
    REQUIRE( board.squares[H8] == std::nullopt );
    REQUIRE( board.squares[F8]->type == ROOK );
    REQUIRE( board.squares[F8]->hasMoved == true );
    REQUIRE( board.blackHasCastled == true );
}

TEST_CASE( "Castle queen side is handled properly", "Board::makeMove()" ) {
    Board board;
    // move away the bishops - dest square cannot be occupied by an allied piece
    board.makeMove( C1, E3, EMPTY );
    board.makeMove( C8, E6, EMPTY );
    // white castle queen side
    board.makeMove( E1, C1, EMPTY );
    REQUIRE( board.squares[A1] == std::nullopt );
    REQUIRE( board.squares[D1]->type == ROOK );
    REQUIRE( board.squares[D1]->hasMoved == true );
    REQUIRE( board.whiteHasCastled == true );
    // black castle king side
    board.makeMove( E8, C8, EMPTY );
    REQUIRE( board.squares[A8] == std::nullopt );
    REQUIRE( board.squares[D8]->type == ROOK );
    REQUIRE( board.squares[D8]->hasMoved == true );
    REQUIRE( board.blackHasCastled == true );
}

TEST_CASE( "Promotion is handled correctly", "Board::makeMove()" ) {
    Board board;
    board.makeMove( H2, H8, KNIGHT );
    REQUIRE( board.squares[H8]->type == KNIGHT );
    REQUIRE( board.squares[H8]->value == KNIGHT_VALUE );
    REQUIRE( board.squares[H8]->actionValue == KNIGHT_ACTION_VALUE );
}

TEST_CASE( "Pieces are moved correctly", "[Board::makeMove()]" ) {
    Board board;
    board.makeMove( F2, F4, EMPTY );
    REQUIRE( board.squares[F2] == std::nullopt );
    REQUIRE( board.squares[F4]->type == PAWN );

    board.makeMove( G8, F6, EMPTY );
    REQUIRE( board.squares[G8] == std::nullopt );
    REQUIRE( board.squares[F6]->type == KNIGHT );
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
/**
 * @file Piece.cc
 * @author Jakub Dydyński
 * @brief Implementation of Piece class. Represents a single piece on the board.
 * @date 2023-06-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Piece.h"

#include <stdexcept>

/* ------------------------------ Constructors ------------------------------ */

Piece::Piece( PieceColor color, PieceType type, bool hasMoved )
    : color( color ), type( type ), hasMoved( hasMoved ), attackedValue( 0 ), defendedValue( 0 ) {
    value = calculatePieceValue( type );
    actionValue = calculatePieceActionValue( type );
}

// Creates a new piece based on char notation (ex. 'Q' for white queen, 'k' for black king)
Piece::Piece( char piece ) : hasMoved( false ), attackedValue( 0 ), defendedValue( 0 ) {
    color = isupper( piece ) ? WHITE : BLACK;
    piece = tolower( piece );
    switch ( piece ) {
        case 'p':
            type = PAWN;
            break;

        case 'n':
            type = KNIGHT;
            break;

        case 'b':
            type = BISHOP;
            break;

        case 'r':
            type = ROOK;
            break;

        case 'q':
            type = QUEEN;
            break;

        case 'k':
            type = KING;
            break;

        default:
            throw std::invalid_argument( "Invalid piece type" );
            break;
    }
    value = calculatePieceValue( type );
    actionValue = calculatePieceActionValue( type );
}

std::string Piece::toString() const {
    std::string pieceString;
    switch ( type ) {
        case PAWN:
            pieceString = "p";
            break;
        case KNIGHT:
            pieceString = "n";
            break;
        case BISHOP:
            pieceString = "b";
            break;
        case ROOK:
            pieceString = "r";
            break;
        case QUEEN:
            pieceString = "q";
            break;
        case KING:
            pieceString = "k";
            break;
        default:
            throw std::logic_error( "Invalid piece type!" );
    }
    if ( color == WHITE ) {
        pieceString = toupper( pieceString[0] );
    }

    return pieceString;
}

/* ----------------------------- Helper methods ----------------------------- */

int Piece::calculatePieceValue( PieceType piece ) {
    switch ( piece ) {
        case PAWN:
            return PAWN_VALUE;
            break;

        case KNIGHT:
            return KNIGHT_VALUE;
            break;

        case BISHOP:
            return BISHOP_VALUE;
            break;

        case ROOK:
            return ROOK_VALUE;
            break;

        case QUEEN:
            return QUEEN_VALUE;
            break;

        case KING:
            return KING_VALUE;
            break;

        default:
            throw std::logic_error( "Invalid piece type!" );
    }
}

int Piece::calculatePieceActionValue( PieceType piece ) {
    switch ( piece ) {
        case PAWN:
            return PAWN_ACTION_VALUE;
            break;

        case KNIGHT:
            return KNIGHT_ACTION_VALUE;
            break;

        case BISHOP:
            return BISHOP_ACTION_VALUE;
            break;

        case ROOK:
            return ROOK_ACTION_VALUE;
            break;

        case QUEEN:
            return QUEEN_ACTION_VALUE;
            break;

        case KING:
            return KING_ACTION_VALUE;
            break;

        default:
            throw std::logic_error( "Invalid piece type!" );
    }
}

#include <stdexcept>

#include "Piece.h"

Piece::Piece( PieceColor color, PieceType type, bool hasMoved )
    : type( type ), hasMoved( hasMoved ), attackedValue( 0 ), defendedValue( 0 ), _color( color ) {
    setPieceValue();
    setPieceActionValue();
}

Piece::Piece( char piece ) : hasMoved( false ), attackedValue( 0 ), defendedValue( 0 ) {
    _color = isupper( piece ) ? WHITE : BLACK;
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
    setPieceValue();
    setPieceActionValue();
}

PieceColor Piece::getColor() const { return _color; }
int Piece::getValue() const { return _value; }
int Piece::getActionValue() const { return _actionValue; }

void Piece::setPieceValue() {
    switch ( type ) {
        case PAWN:
            _value = PAWN_VALUE;
            break;

        case KNIGHT:
            _value = KNIGHT_VALUE;
            break;

        case BISHOP:
            _value = BISHOP_VALUE;
            break;

        case ROOK:
            _value = ROOK_VALUE;
            break;

        case QUEEN:
            _value = QUEEN_VALUE;
            break;

        case KING:
            _value = KING_VALUE;
            break;

        default:
            throw std::logic_error( "Invalid piece type!" );
    }
}

void Piece::setPieceActionValue() {
    switch ( type ) {
        case PAWN:
            _actionValue = PAWN_ACTION_VALUE;
            break;

        case KNIGHT:
            _actionValue = KNIGHT_ACTION_VALUE;
            break;

        case BISHOP:
            _actionValue = BISHOP_ACTION_VALUE;
            break;

        case ROOK:
            _actionValue = ROOK_ACTION_VALUE;
            break;

        case QUEEN:
            _actionValue = QUEEN_ACTION_VALUE;
            break;

        case KING:
            _actionValue = KING_ACTION_VALUE;
            break;

        default:
            throw std::logic_error( "Invalid piece type!" );
    }
}

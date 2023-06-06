#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>

#include "Common.h"

class Piece {
public:
    // constructors
    Piece( PieceColor color, PieceType type, bool hasMoved = false );
    Piece( char piece );

    std::string toString() const;

    // members
    PieceColor color;
    PieceType type;
    bool hasMoved;
    int attackedValue;
    int defendedValue;
    int value;
    int actionValue;
    std::vector<SquareIndex> validMoves;

    // helper methods for creating Pieces
    static int calculatePieceValue( PieceType piece );
    static int calculatePieceActionValue( PieceType piece );
};

#endif
#ifndef MOVE_H
#define MOVE_H

#include <string>

#include "Piece.h"

/* ---------------------------- MoveContent class --------------------------- */

class MoveContent {
public:
    MoveContent();
    MoveContent( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY, PieceType pieceMoving = EMPTY,
                 PieceType pieceTaken = EMPTY, bool isEnPassantCapture = false, int score = 0 );

    SquareIndex src;
    SquareIndex dest;
    PieceType promotion;
    PieceType pieceMoving;
    PieceType pieceTaken;
    bool isEnPassantCapture;
    int score;

    // TODO:
    std::string toPGN() const;

    static int compareMin( const MoveContent &m1, const MoveContent &m2 );
    static int compareMax( const MoveContent &m1, const MoveContent &m2 );
};

#endif
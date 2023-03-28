#ifndef MOVE_H
#define MOVE_H

#include <string>

#include "Piece.h"

/* ---------------------------- MoveContent class --------------------------- */

class MoveContent {
public:
    MoveContent();
    MoveContent( SquareIndex src, SquareIndex dest, PieceType promotion );

    SquareIndex src;
    SquareIndex dest;
    PieceType promotion;
    PieceType pieceMoving;
    PieceType pieceTaken;
    bool isEnPassantCapture;
    int score;

    std::string toPGN() const;
};

#endif

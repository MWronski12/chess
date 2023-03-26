#ifndef ENGINE_H
#define ENGINE_H

#include <stack>

#include "Board.h"
#include "Move.h"
#include "Movegen.h"

class Engine {
public:
    Engine( PieceMovesNestedLists &PieceMoves );
    Board board;
    std::stack<MoveContent> moveHistory;

private:
    PieceMovesNestedLists &movesIterator;
    PieceValidMoves validMoves;
};

#endif
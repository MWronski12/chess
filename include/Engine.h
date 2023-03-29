#ifndef ENGINE_H
#define ENGINE_H

#include <stack>

#include "Board.h"
#include "Move.h"
#include "Movegen.h"

class Engine {
public:
    Engine();
    void newGame();
    void newGame( std::string fen );
    bool makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY );

    Board board;
    PieceValidMoves moveGenerator;
    std::stack<MoveContent> moveHistory;

private:
    Board _previousBoard;
};

#endif
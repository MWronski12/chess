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
    // TODO
    void newGame( std::string fen );
    bool makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY );

    Board board;
    std::stack<MoveContent> moveHistory;
    PieceValidMoves moveGenerator;

private:
    Board _previousBoard;
};

#endif
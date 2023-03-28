#ifndef ENGINE_H
#define ENGINE_H

#include <stack>

#include "Board.h"
#include "Move.h"
#include "Movegen.h"

class Engine {
public:
    void newGame();
    void newGame( std::string fen );
    void makeMove( SquareIndex src, SquareIndex dest );
    PieceColor whoseove();

private:
    Board _board;
    Board _previousBoard;
    PieceValidMoves _moveGenerator;
    std::stack<MoveContent> _moveHistory;
};

#endif
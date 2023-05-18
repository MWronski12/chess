#ifndef ENGINE_H
#define ENGINE_H

#include <chrono>
#include <string>

#include "Board.h"
#include "MoveContent.h"
#include "Movegen.h"
#include "Search.h"

class Engine {
public:
    // Constructors
    Engine();
    Engine( std::string fen );

    // Members
    Board board;

    // Methods
    bool makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY );
    MoveContent getBestMove( int maxDepth = 100, int timeoutSeconds = 5 ) const;

private:
    Search search_;
    PieceValidMoves generator_;
};

#endif
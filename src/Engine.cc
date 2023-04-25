#include "Engine.h"

Engine::Engine() : moveGenerator( PieceValidMoves() ) { newGame(); }

void Engine::newGame() {
    board = std::make_unique<Board>();
    _previousBoard = std::make_unique<Board>();
    moveHistory = std::stack<MoveContent>();

    // Engine always keeps the board with pseudo valid moves calculated!
    moveGenerator.generateValidMoves( *board );
}

PieceColor Engine::whooseMove() const { return board->sideToMove; }

bool Engine::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    std::unique_ptr<Board> currentBoard = std::make_unique<Board>( *board );

    board->makeMove( src, dest, promotion );
    moveGenerator.generateValidMoves( *board );

    if ( !moveGenerator.validateBoard( *board ) ) {
        board = std::move( currentBoard );
        return false;
    }

    moveHistory.push( board->lastMove );
    _previousBoard = std::move( currentBoard );

    return true;
}

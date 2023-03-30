/*
 * Brief: Engine class whose responsibility is to use all classes to create chess engine
 *
 * Author: Jakub Dydynski
 * Date: 26.03.2023
 */

#include "Engine.h"

Engine::Engine() : moveGenerator( PieceValidMoves() ) { newGame(); }

void Engine::newGame() {
    board = Board();
    _previousBoard = Board();
    moveHistory = std::stack<MoveContent>();

    // Engine always keeps the board with pseudo valid moves calculated!
    moveGenerator.generateValidMoves( board );
}

bool Engine::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    // Cache state of the board
    Board temp( board );

    // Make move and recalculate pseudo valid moves
    board.makeMove( src, dest, promotion );
    moveGenerator.generateValidMoves( board );

    // If the move leaves the king in check, it is invalid
    if ( ( board.sideToMove == BLACK && board.whiteIsChecked ) ||
         ( board.sideToMove == WHITE && board.blackIsChecked ) ) {
        board = temp;  // Restore previous board
        return false;
    }

    // Add move to the history
    moveHistory.push( board.lastMove );
    // Update previous board
    _previousBoard = temp;
    return true;
}

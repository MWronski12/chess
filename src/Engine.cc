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
}

bool Engine::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    // Cache state of the board
    Board temp( board );

    // Make move
    board.makeMove( src, dest, promotion );

    // Check if move didnt leave the king in check
    moveGenerator.generateValidMoves( board );
    if ( board.sideToMove == BLACK && board.whiteIsChecked ) {
        board = temp;
        return false;
    }
    if ( board.sideToMove == WHITE && board.blackIsChecked ) {
        board = temp;
        return false;
    }

    // Add move to the history
    moveHistory.push( MoveContent( src, dest, promotion ) );

    // Update previous board
    _previousBoard = temp;

    return true;
}

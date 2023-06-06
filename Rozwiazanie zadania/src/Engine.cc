/*
 * Brief:
 * Engine is a class that encapsulates the Board object and provides an API for playing the game of chess.
 *
 * It is responsible for providing GUI with everything it needs to display ongoing game information.
 * It provides methods for making a move and getting the best possible move for current state of the game.
 * It assures that the stored board is always in a valid state and has pseudo-valid move vectors generated.
 *
 * Author: Mikołaj Wroński
 * Date: 18.05.2023
 */

#include "Engine.h"

Engine::Engine() : board( Board() ), search_( Search() ), generator_( PieceValidMoves() ) {
    generator_.generateValidMoves( board );
    board.score = Evaluation::evaluateBoard( board );
}

Engine::Engine( std::string fen ) : board( Board( fen ) ), search_( Search() ), generator_( PieceValidMoves() ) {
    generator_.generateValidMoves( board );
    board.score = Evaluation::evaluateBoard( board );
}

bool Engine::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    auto boardCopy = board;
    board.makeMove( src, dest, promotion );
    generator_.generateValidMoves( board );
    if ( generator_.validateBoard( board ) == false ) {
        board = boardCopy;
        return false;
    }
    board.score = Evaluation::evaluateBoard( board );
    return true;
}

MoveContent Engine::getBestMove( int maxDepth, int timeoutSeconds ) const {
    auto bestMove = search_.getBestMove( board, maxDepth, board.sideToMove == WHITE, timeoutSeconds );
    return bestMove;
}

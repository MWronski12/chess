#ifndef ENGINE_H
#define ENGINE_H

#include <chrono>
#include <string>

#include "Board.h"
#include "MoveContent.h"
#include "Movegen.h"
#include "Search.h"

// Engine has to provide all the crucial functions to GUI:
// - Start new game (player vs computer)
// - end the game (for resign, out of time loss)
// - request making a move
// - request undoing a move
//
// - set timeout for move searching
// - navigate move history
//      - request current position
// - evaluation
// - game result
// - whoose move
// - moves possible for each side
// - best move for the current player
//

class Engine {
public:
    // Constructors
    Engine() : board( Board() ), search_( Search() ), generator_( PieceValidMoves() ) {
        /* -------------------------------------------------------------------------- */
        board = Board();
        generator_.generateValidMoves( board );
    };

    // Members
    Board board;

    // Methods
    bool makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY ) {
        auto boardCopy = board;
        board.makeMove( src, dest, promotion );
        generator_.generateValidMoves( board );
        if ( generator_.validateBoard( board ) == false ) {
            board = boardCopy;
            return false;
        }
        return true;
    };

    int getEvaluation() const { return Evaluation::evaluateBoard( board ); };

    std::string getBestMove( int maxDepth = 10, int timeoutSeconds = 5 ) const {
        auto bestMove = search_.getBestMove( board, maxDepth, board.sideToMove == WHITE, timeoutSeconds );
        return bestMove.toSAN();
    };

private:
    Search search_;
    PieceValidMoves generator_;
};

#endif
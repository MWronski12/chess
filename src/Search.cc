#include <algorithm>
#include <limits>

#include "Search.h"

/**
 * Returns the best possible move for the current player.
 * It assumes that the board has valid moves calculated and the game is not over yet!
 *
 * @param board position to examine.
 * @param maxDepth maximum depth of search.
 * @param maximizingPlayer true if the current player is WHITE, false if BLACK.
 *
 * @return MoveContent representing the best move for the current player.
 */
MoveContent Search::getBestMove( const Board& examineBoard, int maxDepth, bool maximizingPlayer ) const {
    MoveContent bestMove;
    bestMove.score = maximizingPlayer ? NEGATIVE_INFINITY : POSITIVE_INFINITY;
    std::vector<MoveContent> possibleMoves = evaluateMoves( examineBoard );
    auto compare = maximizingPlayer ? MoveContent::compareMax : MoveContent::compareMin;

    // Perform iterative deepening search
    for ( int depth = 1; depth <= maxDepth; depth++ ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), compare );

        for ( auto move : possibleMoves ) {
            Board board = examineBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            move.score = alphaBeta( board, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, !maximizingPlayer );

            if ( ( maximizingPlayer && move.score > bestMove.score ) ||
                 ( !maximizingPlayer && move.score < bestMove.score ) ) {
                bestMove = move;
            }
        }
        // TODO: Should be possible to terminate the search at any given time
        // if ( timeIsUp() ) {
        //     break;
        // }
    }
    return bestMove;
}

/**
 * Calculates the score for the current board and player, recursively searching the game tree.
 *
 * @param board position to examine.
 * @param depth maximum depth of search.
 * @param alpha maximizing player best score.
 * @param beta minimizing player best score.
 * @param maximizingPlayer true if the current player is WHITE, false if BLACK.
 *
 * @return int score for the current board and player.
 */
int Search::alphaBeta( const Board& examineBoard, int depth, int alpha, int beta, bool maximizingPlayer ) const {
    if ( depth == 0 ) {
        return Evaluation::evaluateBoard( examineBoard );
    }

    // If no legal moves found we decide that the game is over.
    bool isEndOfTheGame = true;

    std::vector<MoveContent> possibleMoves = evaluateMoves( examineBoard );

    /* ---------------------------- Maximizing Player --------------------------- */
    if ( maximizingPlayer ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMax );

        for ( auto move : possibleMoves ) {
            auto board = examineBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            // We found a legal move, the game is not over.
            isEndOfTheGame = false;
            int eval = alphaBeta( board, depth - 1, alpha, beta, false );
            alpha = std::max( alpha, eval );
            if ( beta <= alpha ) {
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( examineBoard );

        return alpha;
    }
    /* ---------------------------- Minimizing player --------------------------- */
    else {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMin );

        for ( auto move : possibleMoves ) {
            auto board = examineBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            isEndOfTheGame = false;
            int eval = alphaBeta( board, depth - 1, alpha, beta, true );
            beta = std::min( beta, eval );
            if ( beta <= alpha ) {
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( examineBoard );

        return beta;
    }
}

/**
 * Calculates the score for the end of the game.
 * Assumes that the given board represents a game over.
 *
 * @param board position to examine.
 *
 * @return int score for the end of the game.
 */
int Search::endOfTheGameScore( const Board& board ) const {
    // White is check mated
    if ( board.sideToMove == WHITE && board.whiteIsChecked ) {
        return NEGATIVE_INFINITY;
    }
    // Black is check mated
    else if ( board.sideToMove == BLACK && board.blackIsChecked ) {
        return POSITIVE_INFINITY;
    }
    // Stale mate
    else {
        return 0;
    }
}

/**
 * Generates a list of pseudo evaluated possible moves sorted by highest scored first.
 * Pseudo evaluation tries to guess which moves are the most promising, so they can be searched first.
 * It assumes that the board has valid moves calculated.
 * Considerations: captures only (TODO: promotion, enpassant, castling and piece's first move).
 *
 * @param board Board to examine, it has to have valid moves calculated.
 *
 * @return vector of pseudo evaluated moves for the current board sorted by highest scored first.
 */
std::vector<MoveContent> Search::getPossibleMoves( const Board& board ) const {
    std::vector<MoveContent> moves;

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        const auto& pieceMoving = board.squares[srcSquare];
        if ( pieceMoving == std::nullopt || pieceMoving->color != board.sideToMove ) continue;

        MoveContent move;
        move.src = srcSquare;
        for ( auto destSquare : pieceMoving->validMoves ) {
            move.dest = destSquare;
            move.pieceMoving = pieceMoving->type;
            const auto& pieceTaken = board.squares[destSquare];
            pieceTaken ? move.pieceTaken = pieceTaken->type : move.pieceTaken = EMPTY;

            /* -------------------------------- Captures -------------------------------- */
            if ( pieceTaken ) {
                move.score += CAPTURE_MOVE_REWARD;
                move.score += pieceMoving->actionValue - pieceTaken->actionValue;     // Lowest value attacker
                move.score += pieceTaken->attackedValue - pieceTaken->defendedValue;  // Highest value attacked
            }

            moves.push_back( move );
        }
    }

    std::sort( moves.begin(), moves.end(), []( MoveContent m1, MoveContent m2 ) { return m1.score > m2.score; } );
    return moves;
}
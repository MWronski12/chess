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
MoveContent Search::getBestMove( const Board& board, int maxDepth, bool maximizingPlayer ) const {
    MoveContent bestMove;
    bestMove.score = maximizingPlayer ? NEGATIVE_INFINITY : POSITIVE_INFINITY;
    std::vector<MoveContent> possibleMoves = evaluateMoves( board );
    auto compare = maximizingPlayer ? MoveContent::compareMax : MoveContent::compareMin;

    // Perform iterative deepening search
    for ( int depth = 1; depth <= maxDepth; depth++ ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), compare );

        for ( auto move : possibleMoves ) {
            Board boardCopy = board.fastCopy();
            boardCopy.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( boardCopy );
            if ( !generator.validateBoard( boardCopy ) ) {
                continue;
            }

            move.score = alphaBeta( boardCopy, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, !maximizingPlayer );

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
int Search::alphaBeta( const Board& board, int depth, int alpha, int beta, bool maximizingPlayer ) const {
    if ( depth == 0 ) {
        return Evaluation::evaluateBoard( board );
    }

    // If no legal moves found we decide that the game is over.
    bool isEndOfTheGame = true;

    std::vector<MoveContent> possibleMoves = evaluateMoves( board );

    /* ---------------------------- Maximizing Player --------------------------- */
    if ( maximizingPlayer ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMax );

        for ( auto move : possibleMoves ) {
            Board boardCopy = board.fastCopy();
            boardCopy.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( boardCopy );
            if ( !generator.validateBoard( boardCopy ) ) {
                continue;
            }

            // We found a legal move, the game is not over.
            isEndOfTheGame = false;
            int eval = alphaBeta( boardCopy, depth - 1, alpha, beta, false );
            alpha = std::max( alpha, eval );
            if ( beta <= alpha ) {
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( board );

        return alpha;
    }
    /* ---------------------------- Minimizing player --------------------------- */
    else {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMin );

        for ( auto move : possibleMoves ) {
            Board boardCopy = board.fastCopy();
            boardCopy.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( boardCopy );
            if ( !generator.validateBoard( boardCopy ) ) {
                continue;
            }

            isEndOfTheGame = false;
            int eval = alphaBeta( boardCopy, depth - 1, alpha, beta, true );
            beta = std::min( beta, eval );
            if ( beta <= alpha ) {
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( board );

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
 * Generates a list of pseudo evaluated possible moves for the current board position.
 * Pseudo evaluation tries to guess which moves are the most promising, so they can be searched first.
 * It assumes that the board has valid moves calculated.
 * Considerations: promotions, captures, enpassant captures (TODO: castling and piece first move).
 *
 * @param board Board to examine, it has to have valid moves calculated.
 *
 * @return vector of pseudo evaluated moves for the current board.
 */
std::vector<MoveContent> Search::evaluateMoves( const Board& board ) const {
    std::vector<MoveContent> moves;

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto pieceMoving = board.squares[srcSquare];
        if ( pieceMoving == std::nullopt || pieceMoving->getColor() != board.sideToMove ) continue;

        MoveContent move;
        move.src = srcSquare;
        for ( auto destSquare : pieceMoving->validMoves ) {
            move.dest = destSquare;
            move.pieceMoving = pieceMoving->type;
            auto pieceTaken = board.squares[destSquare];
            pieceTaken ? move.pieceTaken = pieceTaken->type : move.pieceTaken = EMPTY;

            /* ----------------------------- Promotion moves ---------------------------- */
            if ( pieceMoving->type == PAWN && ( destSquare < 8 || destSquare > 55 ) ) {
                for ( auto promotion : { QUEEN, ROOK, BISHOP, KNIGHT } ) {
                    move.promotion = promotion;
                    switch ( promotion ) {
                        case QUEEN:
                            move.score += QUEEN_VALUE;
                            break;
                        case ROOK:
                            move.score += ROOK_VALUE;
                            break;
                        case BISHOP:
                            move.score += BISHOP_VALUE;
                            break;
                        case KNIGHT:
                            move.score += KNIGHT_VALUE;
                            break;
                        default:
                            break;
                    }
                    moves.push_back( move );
                }
            }
            /* -------------------------------- Captures -------------------------------- */
            else if ( pieceTaken ) {
                move.pieceTaken = pieceTaken->type;
                move.score += CAPTURE_MOVE_REWARD;
                // Reward capturing with lowest valued piece
                move.score += pieceMoving->getActionValue() - pieceTaken->getActionValue();
                // Reward capturing undefended pieces
                move.score += pieceTaken->attackedValue - pieceTaken->defendedValue;
                moves.push_back( move );
            }
            /* --------------------------- En passant captures -------------------------- */
            else if ( pieceMoving->type == PAWN && destSquare == board.getEnPassantSquare() ) {
                move.isEnPassantCapture = true;
                move.pieceTaken = PAWN;
                move.score += CAPTURE_MOVE_REWARD;
                moves.push_back( move );
            }
            /* ------------------------------- Normal move ------------------------------ */
            else {
                move.score = 0;
                moves.push_back( move );
            }
        }
    }
    return moves;
}
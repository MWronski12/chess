#include "Search.h"

#include <algorithm>
#include <limits>
#include <optional>

#include "TranspositionTable.h"

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
MoveContent Search::getBestMove( Board& examineBoard, int maxDepth, bool maximizingPlayer ) {
    MoveContent bestMove;
    bestMove.score = maximizingPlayer ? NEGATIVE_INFINITY : POSITIVE_INFINITY;
    std::vector<MoveContent> possibleMoves = evaluateMoves( examineBoard );
    auto compare = maximizingPlayer ? MoveContent::compareMax : MoveContent::compareMin;

    // std::optional<TranspositionEntry> entry = tt.getEntry( examineBoard.hash_key );
    // if ( entry.has_value() && entry.value().depth >= maxDepth && entry.value().NodeType == EXACT ) {
    //     return entry.value().best_move.value();
    // }
    // change so that we can know depth // to raczej trzeba wywołać w alfa beta
    // This position has already been searched to the desired depth or deeper, so we can use the cached score.

    // Perform iterative deepening search
    for ( int depth = 1; depth <= maxDepth; depth++ ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), compare );

        for ( auto move : possibleMoves ) {
            Board board = examineBoard;  // thingk about it bcs we copy some things that are irrelevant
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            move.score = alphaBeta( board, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, !maximizingPlayer );

            if ( ( maximizingPlayer && move.score > bestMove.score ) || ( !maximizingPlayer && move.score < bestMove.score ) ) {
                // tt.putEntry( board.hash_key, move.score, depth, EXACT, move );
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
int Search::alphaBeta( Board& examineBoard, int depth, int alpha, int beta, bool maximizingPlayer ) {
    if ( depth == 0 ) {
        return Evaluation::evaluateBoard( examineBoard );
    }

    // std::optional<TranspositionEntry> entry = tt.getEntry( examineBoard.hash_key );
    // if ( entry.has_value() && entry.value().depth >= depth && entry.value().NodeType == EXACT ) {
    //     return entry.value().score;
    // } else if ( entry.has_value() && entry.value().depth >= depth && entry.value().score >= beta &&
    //             entry.value().NodeType == LOWERBOUND ) {
    //     return beta;
    // } else if ( entry.has_value() && entry.value().depth >= depth && entry.value().score <= alpha &&
    //             entry.value().NodeType == LOWERBOUND ) {
    //     return alpha;
    // }

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
                // tt.putEntry( board.hash_key, alpha, depth, UPPERBOUND, std::nullopt );
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
                // tt.putEntry( board.hash_key, beta, depth, LOWERBOUND, std::nullopt );
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
int Search::endOfTheGameScore( Board& board ) {
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
std::vector<MoveContent> Search::evaluateMoves( Board& board ) {
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
                move.score += pieceMoving->actionValue - pieceTaken->actionValue;
                // Reward capturing undefended pieces
                move.score += pieceTaken->attackedValue - pieceTaken->defendedValue;
                moves.push_back( move );
            }
            /* --------------------------- En passant captures -------------------------- */
            else if ( pieceMoving->type == PAWN && destSquare == board.enPassantSquare ) {
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

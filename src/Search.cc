#include <algorithm>
#include <chrono>
#include <iostream>
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
MoveContent Search::getBestMove( const Board& examinedBoard, int maxDepth, bool maximizingPlayer, int timeout ) const {
    auto start = std::chrono::steady_clock::now();

    int nodesExamined = 0;
    int nodesEvaluated = 0;
    int nodesPruned = 0;
    int nodesExaminedQ = 0;
    int nodesEvaluatedQ = 0;
    int nodesPrunedQ = 0;

    MoveContent bestMove;
    bestMove.score = maximizingPlayer ? NEGATIVE_INFINITY : POSITIVE_INFINITY;
    std::vector<MoveContent> possibleMoves = getPossibleMoves( examinedBoard );

    auto compare = maximizingPlayer ? MoveContent::compareMax : MoveContent::compareMin;

    // Perform iterative deepening search
    int depth = 1;
    for ( ; depth <= maxDepth; depth++ ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), compare );

        for ( auto move : possibleMoves ) {
            Board board = examinedBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            // Break if it passed more than timeout seconds since start
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - start ).count();
            if ( duration >= timeout ) {
                break;
            }

            move.score =
                alphaBeta( board, depth, NEGATIVE_INFINITY, POSITIVE_INFINITY, !maximizingPlayer, nodesExamined,
                           nodesEvaluated, nodesPruned, nodesExaminedQ, nodesEvaluatedQ, nodesPrunedQ );

            // Found a better move
            if ( ( maximizingPlayer && move.score > bestMove.score ) ||
                 ( !maximizingPlayer && move.score < bestMove.score ) ) {
                bestMove = move;
            }

            // Found a forced mate
            if ( ( maximizingPlayer && bestMove.score == POSITIVE_INFINITY ) ||
                 ( !maximizingPlayer && bestMove.score == NEGATIVE_INFINITY ) ) {
                return bestMove;
            }
        }

        // Break if it passed more than timeout seconds since start
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>( end - start ).count();
        if ( duration >= timeout ) {
            break;
        }
    }

    std::cout << "Nodes examined: " << nodesExamined << std::endl;
    std::cout << "Nodes evaluated: " << nodesEvaluated << std::endl;
    std::cout << "Nodes pruned: " << nodesPruned << std::endl;
    std::cout << "Depth searched: " << depth << std::endl;
    std::cout << std::endl;
    std::cout << "Nodes examined Q: " << nodesExaminedQ << std::endl;
    std::cout << "Nodes evaluated Q: " << nodesEvaluatedQ << std::endl;
    std::cout << "Nodes pruned Q:" << nodesPrunedQ << std::endl;

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
int Search::alphaBeta( const Board& examinedBoard, int depth, int alpha, int beta, bool maximizingPlayer,
                       int& nodesExamined, int& nodesEvaluated, int& nodesPruned, int& nodesExaminedQ,
                       int& nodesEvaluatedQ, int& nodesPrunedQ ) const {
    nodesExamined++;
    if ( depth == 0 ) {
        nodesEvaluated++;
        return quiescentSearch( examinedBoard, depth / 2 + 1, alpha, beta, maximizingPlayer, nodesExaminedQ,
                                nodesEvaluatedQ, nodesPrunedQ );
    }

    // If no legal moves found we decide that the game is over.
    bool isEndOfTheGame = true;

    std::vector<MoveContent> possibleMoves = getPossibleMoves( examinedBoard );

    /* ---------------------------- Maximizing Player --------------------------- */
    if ( maximizingPlayer ) {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMax );

        for ( auto move : possibleMoves ) {
            auto board = examinedBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            // We found a legal move, the game is not over.
            isEndOfTheGame = false;
            int eval = alphaBeta( board, depth - 1, alpha, beta, false, nodesExamined, nodesEvaluated, nodesPruned,
                                  nodesExaminedQ, nodesEvaluatedQ, nodesPrunedQ );
            alpha = std::max( alpha, eval );
            if ( beta <= alpha ) {
                nodesPruned++;
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( examinedBoard );

        return alpha;
    }
    /* ---------------------------- Minimizing player --------------------------- */
    else {
        std::sort( possibleMoves.begin(), possibleMoves.end(), MoveContent::compareMin );

        for ( auto move : possibleMoves ) {
            auto board = examinedBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            isEndOfTheGame = false;
            int eval = alphaBeta( board, depth - 1, alpha, beta, true, nodesExamined, nodesEvaluated, nodesPruned,
                                  nodesExaminedQ, nodesEvaluatedQ, nodesPrunedQ );
            beta = std::min( beta, eval );
            if ( beta <= alpha ) {
                nodesPruned++;
                break;
            }
        }
        if ( isEndOfTheGame ) return endOfTheGameScore( examinedBoard );

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
 * Generates a list of pseudo evaluated possible moves for the given board.
 * Pseudo evaluation tries to guess which moves are the most promising, so they can be searched first.
 * Moves score for black is negative and for white is positive (black's best move is -inf).
 * It assumes that the board has valid moves calculated.
 * Considerations: captures only (TODO: promotion, enpassant, castling and piece's first move).
 *
 * @param board Board to examine, it has to have valid moves calculated.
 *
 * @return vector of pseudo evaluated moves for the current board.
 */
std::vector<MoveContent> Search::getPossibleMoves( const Board& board ) const {
    std::vector<MoveContent> moves;

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        const auto& pieceMoving = board.squares[srcSquare];
        if ( pieceMoving == std::nullopt || pieceMoving->color != board.sideToMove ) continue;

        MoveContent move;
        move.src = srcSquare;
        move.pieceMoving = pieceMoving->type;

        for ( auto destSquare : pieceMoving->validMoves ) {
            move.dest = destSquare;
            if ( pieceMoving->type == PAWN && ( destSquare < 8 || destSquare > 55 ) ) {
                move.promotion = QUEEN;
            }

            const auto& pieceTaken = board.squares[destSquare];
            pieceTaken ? move.pieceTaken = pieceTaken->type : move.pieceTaken = EMPTY;

            /* -------------------------------- Captures -------------------------------- */
            if ( pieceTaken ) {
                move.score += CAPTURE_MOVE_REWARD;
                move.score += pieceMoving->actionValue - pieceTaken->actionValue;     // Lowest value attacker
                move.score += pieceTaken->attackedValue - pieceTaken->defendedValue;  // Highest value attacked
            }

            if ( board.sideToMove == BLACK ) move.score = -move.score;
            moves.push_back( move );
        }
    }

    return moves;
}

/**
 * Generates a list of available capture moves for the given board.
 * Moves are sorted such that captueres of highest value piece with lowest value piece are first.
 *
 * @param board Board to examine, it has to have valid moves calculated.
 *
 * @return vector of available capture moves for the given Board sorted by score.
 */
std::vector<MoveContent> Search::getPossibleCaptureMoves( const Board& board ) const {
    std::vector<MoveContent> moves;

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        const auto& pieceMoving = board.squares[srcSquare];
        if ( pieceMoving == std::nullopt || pieceMoving->color != board.sideToMove ) continue;

        for ( auto destSquare : pieceMoving->validMoves ) {
            const auto& pieceTaken = board.squares[destSquare];
            if ( pieceTaken ) {
                MoveContent move;
                move.src = srcSquare;
                move.dest = destSquare;
                move.pieceMoving = pieceMoving->type;
                move.pieceTaken = pieceTaken->type;
                if ( pieceMoving->type == PAWN && ( destSquare < 8 || destSquare > 55 ) ) {
                    move.promotion = QUEEN;
                }
                // Prefer capturing higher value pieces with lower value pieces.
                move.score = pieceTaken->value - pieceMoving->value;
                moves.push_back( move );
            }
        }
    }

    std::sort( moves.begin(), moves.end(), MoveContent::compareMax );
    return moves;
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
int Search::quiescentSearch( const Board& examinedBoard, int depth, int alpha, int beta, bool maximizingPlayer,
                             int& nodesExamined, int& nodesEvaluated, int& nodesPruned ) const {
    nodesExamined++;
    std::vector<MoveContent> possibleCaptureMoves = getPossibleCaptureMoves( examinedBoard );

    if ( depth == 0 || possibleCaptureMoves.empty() ) {
        nodesEvaluated++;
        return Evaluation::evaluateBoard( examinedBoard );
    }

    /* ---------------------------- Maximizing Player --------------------------- */
    if ( maximizingPlayer ) {
        for ( auto move : possibleCaptureMoves ) {
            auto board = examinedBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            int eval =
                quiescentSearch( board, depth - 1, alpha, beta, false, nodesExamined, nodesEvaluated, nodesPruned );
            alpha = std::max( alpha, eval );
            if ( beta <= alpha ) {
                nodesPruned++;
                break;
            }
        }

        return alpha;
    }
    /* ---------------------------- Minimizing player --------------------------- */
    else {
        for ( auto move : possibleCaptureMoves ) {
            auto board = examinedBoard;
            board.makeMove( move.src, move.dest, move.promotion );
            generator.generateValidMoves( board );
            if ( !generator.validateBoard( board ) ) {
                continue;
            }

            int eval =
                quiescentSearch( board, depth - 1, alpha, beta, true, nodesExamined, nodesEvaluated, nodesPruned );
            beta = std::min( beta, eval );
            if ( beta <= alpha ) {
                nodesPruned++;
                break;
            }
        }

        return beta;
    }
}

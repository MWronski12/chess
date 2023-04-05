#include "Search.h"

int Search::miniMaxScore( PieceColor sideToMove, const std::vector<MoveContent>& moves ) {
    if ( sideToMove == WHITE ) {
        auto maxMovePtr = std::max_element( moves.begin(), moves.end(), MoveContent::compare );
        return maxMovePtr->score;
    } else {
        auto minMovePtr = std::min_element( moves.begin(), moves.end(), MoveContent::compare );
        return minMovePtr->score;
    }
}

int Search::miniMax( int depth, const Board& board ) {
    if ( depth == 0 ) return Evaluation::evaluate( board );

    std::vector<MoveContent> moves = evaluateMoves( board );

    for ( auto& move : moves ) {
        Board boardCopy = board.fastCopy();
        boardCopy.makeMove( move.src, move.dest );
        generator.generateValidMoves( boardCopy );
        if ( generator.validateMove( boardCopy ) == false ) {
            continue;
        }
        move.score = miniMax( depth - 1, boardCopy );
    }

    return miniMaxScore( board.sideToMove, moves );
}

MoveContent Search::miniMaxMove( PieceColor sideToMove, const std::vector<MoveContent>& moves ) {
    if ( sideToMove == WHITE ) {
        return *std::max_element( moves.begin(), moves.end(), MoveContent::compare );
    } else {
        return *std::min_element( moves.begin(), moves.end(), MoveContent::compare );
    }
}

MoveContent Search::getBestMove( const Board& board, int depth ) {
    std::vector<MoveContent> moves = evaluateMoves( board );
    for ( auto& move : moves ) {
        move.score = miniMax( depth - 1, board );
    }
    return miniMaxMove( board.sideToMove, moves );
}

std::vector<MoveContent> Search::evaluateMoves( const Board& board ) {
    // CONSIDERATIONS:
    // promotions
    //  --> QUEEN first
    // captures
    //  --> difference in pieceValue
    //  --> difference in pieceAttacked/Defended value
    // enpassant captures

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
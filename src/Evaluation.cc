#include <numeric>

#include "Evaluation.h"

int Evaluation::evaluateBoard( const Board &board ) {
    int score = 0;

    for ( SquareIndex square = 0; square < 64; square++ ) {
        if ( board.squares[square] != std::nullopt ) {
            const auto piece = board.squares[square];
            const auto color = piece->color;

            /* ----------------------- Accumulate material balance ---------------------- */
            score += color == WHITE ? piece->value : -piece->value;

            /* ------------------------- Evaluate piece position ------------------------ */
            switch ( piece->type ) {
                case PAWN:
                    score += color == WHITE ? PAWN_TABLE[square] : -PAWN_TABLE[63 - square];
                    break;
                case KNIGHT:
                    score += color == WHITE ? KNIGHT_TABLE[square] : -KNIGHT_TABLE[63 - square];
                    break;
                case BISHOP:
                    score += color == WHITE ? BISHOP_TABLE[square] : -BISHOP_TABLE[63 - square];
                    break;
                case KING:
                    score += color == WHITE ? KING_TABLE[square] : -KING_TABLE[63 - square];
                    break;
                default:
                    break;
            }
        }
    }
    return score;
}

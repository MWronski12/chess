#include <numeric>

#include "Evaluation.h"

int Evaluation::evaluate( const Board &board ) {
    // Accumulate material balance
    return std::accumulate(
        board.squares.begin(), board.squares.end(), 0, []( int score, std::optional<Piece> square ) -> auto {
            if ( square.has_value() ) {
                return square->getColor() == WHITE ? score + square->getValue() : score - square->getValue();
            }
            return score;
        } );
}

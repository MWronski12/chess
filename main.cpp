#include <iostream>

#include "Movegen.h"

using namespace std;

int main() {
    PieceMoves &pieceMoves = PieceMoves::getInstance();
    int count = 0;

    for ( SquareIndex square = 0; square < 64; square++ )
        for ( auto &ray : pieceMoves.getMoveList( WHITE, PAWN, square ) ) {
            for ( auto &_ : ray ) {
                (void)_;
                count++;
            }
        }

    cout << count << endl;

    return 1;
}

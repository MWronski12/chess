#include <iostream>

#include "Engine.h"

using namespace std;

uint64_t perft( int depth, Engine &e ) {
    uint64_t nodes = 0;

    if ( depth == 0 ) {
        return 1;
    }

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = e.board->squares[srcSquare];

        if ( piece == std::nullopt || piece->getColor() != e.whooseMove() ) {
            continue;
        }

        for ( auto destSquare : piece->validMoves ) {
            std::unique_ptr<Board> root = std::make_unique<Board>( *e.board );
            if ( e.makeMove( srcSquare, destSquare ) == true ) {
                nodes += perft( depth - 1, e );
                e.board = std::move( root );
            }
        }
    }

    return nodes;
}

int main() {
    Engine e;

    cout << "Perft(3) == " << perft( 3, e ) << endl;

    return 0;
}

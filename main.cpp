#include <iostream>

#include "Engine.h"

using namespace std;

uint64_t perft( uint8_t depth, Engine& engine, PieceValidMoves& moveGenerator ) {
    if ( depth == 0 ) {
        return 1;
    }
    uint64_t nodes = 0;

    moveGenerator.generateValidMoves( engine.board );

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = engine.board.squares[srcSquare];
        if ( !piece || piece->getColor() != engine.board.sideToMove ) {
            continue;
        }

        for ( auto& destSquare : piece->validMoves ) {
            Board prevBoard = engine.board;

            if ( engine.makeMove( srcSquare, destSquare ) ) {
                nodes += perft( depth - 1, engine, moveGenerator );
            }
            engine.board = prevBoard;
        }
    }

    return nodes;
}

int main() {
    Engine engine;
    PieceValidMoves moveGenerator;
    cout << perft( 2, engine, moveGenerator ) << endl;
    return 0;
}

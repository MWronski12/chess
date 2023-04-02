#include <iostream>

#include "Board.h"
#include "Movegen.h"

using namespace std;

uint64_t perft( int depth, Board &board, PieceValidMoves &generator ) {
    if ( depth == 0 ) {
        return 1;
    }

    uint64_t nodes = 0;

    // Cache the board
    Board currentBoard = Board( board );

    // Iterate through every piece on the board
    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto piece = board.squares[srcSquare];

        if ( piece == std::nullopt || piece->getColor() != board.sideToMove ) {
            continue;
        }

        // Iterate through every pseudo valid move for the piece
        for ( auto destSquare : piece->validMoves ) {
            // Make the move
            board.makeMove( srcSquare, destSquare );
            generator.generateValidMoves( board );

            // Count the subnodes count if the move is valid
            if ( !( board.blackIsChecked && board.sideToMove == WHITE ) &&
                 !( board.whiteIsChecked && board.sideToMove == BLACK ) ) {
                nodes += perft( depth - 1, board, generator );
            }

            // Undo the move
            board = currentBoard;
        }
    }

    return nodes;
}

int main() {
    Board b;
    PieceValidMoves g;
    g.generateValidMoves( b );
    int depth = 4;
    uint64_t nodes = perft( depth, b, g );
    cout << "Perft(" << depth << ") == " << nodes << endl;

    return 0;
}

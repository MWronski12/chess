#include <iostream>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 5, b.sideToMove == WHITE );

    cout << "Piece moved: " << bestMove.pieceMoving << endl;
    cout << "source: " << (int)bestMove.src << endl;
    cout << "dest: " << (int)bestMove.dest << endl;

    return 0;
}

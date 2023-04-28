#include <iostream>

#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    // Gui *gui = new ConsoleGui();
    // gui->start();

    Search s;
    Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    s.getBestMove( b, 2, b.sideToMove == WHITE );

    return 0;
}

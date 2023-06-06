#include <iostream>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    Search s;
    Board b( "3r4/pR2N3/2pkb3/5p2/8/2B5/qP3PPP/4R1K1 w - - 1 0" );
    PieceValidMoves g;
    g.generateValidMoves( b );
    auto bestMove = s.getBestMove( b, 10, b.sideToMove == WHITE );
    cout << "Best move: " << bestMove.toString() << endl;

    // Gui *gui = new ConsoleGui();
    // gui->start();
    WindowGui gui = WindowGui();
    gui.start();
    return 0;
}

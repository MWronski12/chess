#include <iostream>

#include "Engine.h"
#include "Gui.h"

using namespace std;
using namespace sf;

int main() {
    // PieceMoves &pieceMoves = PieceMoves::getInstance();
    // int count = 0;

    // for ( SquareIndex square = 0; square < 64; square++ )
    //     for ( auto &ray : pieceMoves.getMoveList( WHITE, PAWN, square ) ) {
    //         for ( auto &_ : ray ) {
    //             (void)_;
    //             count++;
    //         }
    //     }

    // cout << count << endl;
    WindowGui wgui = WindowGui();
    wgui.draw();
    while ( true ) {
        // string stri;
        // try {
        //     wgui.makeMove( stri );
        // } catch ( exception &e ) {
        //     cout << "Bad move input!\n";
        //     continue;
        // }
    }

    return 1;
}

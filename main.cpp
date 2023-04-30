#include <iostream>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    // Gui *gui = new ConsoleGui();
    // gui->start();

    // Search s;
    // Board b( "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 2" );
    // PieceValidMoves g;
    // g.generateValidMoves( b );
    // s.getBestMove( b, 2, b.sideToMove == WHITE );

    Board board = Board();
    WindowGui wgui = WindowGui( board );
    wgui.draw();
    while ( wgui.window.isOpen() ) {
        Vector2i pos = Mouse::getPosition( wgui.window ) - Vector2i( wgui.offset );

        Event e;
        while ( wgui.window.pollEvent( e ) ) {
            if ( e.type == Event::Closed ) {
                wgui.window.close();
                break;
            }

            // ////move back//////
            // if (e.type == Event::KeyPressed)
            //   if (e.key.code == Keyboard::BackSpace) {
            //     if (position.length() > 6)
            //       position.erase(position.length() - 6, 5);
            //     loadPosition();
            //   }

            /////drag and drop///////
            wgui.dragAndDrop( e, pos );
        }

        if ( wgui.isMove ) wgui.f[wgui.n].setPosition( pos.x - wgui.dx, pos.y - wgui.dy );
        wgui.draw();
    }
    return 0;
}

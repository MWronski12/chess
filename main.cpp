#include <iostream>
#include <string>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    Search s;
    // Board b( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" );
    // PieceValidMoves g;
    WindowGui wgui = WindowGui();
    wgui.draw();

    while ( wgui.window.isOpen() ) {
        Vector2i pos = Mouse::getPosition( wgui.window ) - Vector2i( wgui.offset );

        Event e;
        while ( wgui.window.pollEvent( e ) ) {
            if ( e.type == Event::Closed ) {
                wgui.window.close();
                break;
            }

            /////drag and drop///////
            wgui.dragAndDrop( e, pos );

            /////comp move/////// press SPACE for comp to move
            wgui.compMove( s );
        }
        wgui.ifIsMoveThenUpdatePosition( pos );
        wgui.draw();
    }
    return 0;
}

#include <iostream>
#include <string>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

std::string toChessNotation( int square ) {
    int rank = 8 - square / 8;
    int file = square % 8;
    std::string notation = "";
    notation += char( file + 'a' );
    notation += char( rank + '0' );
    return notation;
}

std::string toChessNotation( int src, int dest ) {
    std::string srcNotation = toChessNotation( src );
    std::string destNotation = toChessNotation( dest );
    return srcNotation + destNotation;
}

int main() {
    // Gui *gui = new ConsoleGui();
    // gui->start();

    Search s;
    Board b( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" );
    PieceValidMoves g;
    // g.generateValidMoves( b );
    // s.getBestMove( b, 2, b.sideToMove == WHITE );

    // Board board = Board();
    WindowGui wgui = WindowGui( b );
    wgui.draw();

    bool compFlag = false;

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

            // comp
            if ( Keyboard::isKeyPressed( Keyboard::Space ) ) {
                g.generateValidMoves( b );
                MoveContent mov = s.getBestMove( b, 5, b.sideToMove == WHITE );
                string str = toChessNotation( mov.src, mov.dest );

                wgui.oldPos = wgui.toCoord( str[0], str[1] );
                wgui.newPos = wgui.toCoord( str[2], str[3] );

                for ( int i = 0; i < 32; i++ )
                    if ( wgui.f[i].getPosition() == wgui.oldPos ) wgui.n = i;

                /////animation///////
                for ( int k = 0; k < 50; k++ ) {
                    Vector2f p = wgui.newPos - wgui.oldPos;
                    wgui.f[wgui.n].move( p.x / 50, p.y / 50 );
                    wgui.window.draw( wgui.sBoard );
                    for ( int i = 0; i < 32; i++ ) wgui.f[i].move( wgui.offset );
                    for ( int i = 0; i < 32; i++ ) wgui.window.draw( wgui.f[i] );
                    wgui.window.draw( wgui.f[wgui.n] );
                    for ( int i = 0; i < 32; i++ ) wgui.f[i].move( -wgui.offset );
                    wgui.window.display();
                }

                wgui.makeMove( str );
                wgui.position += str + " ";
                wgui.f[wgui.n].setPosition( wgui.newPos );
            }
        }

        if ( wgui.isMove ) wgui.f[wgui.n].setPosition( pos.x - wgui.dx, pos.y - wgui.dy );

        wgui.draw();
    }
    return 0;
}

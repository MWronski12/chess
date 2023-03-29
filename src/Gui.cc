#include "Gui.h"

WindowGui::WindowGui() : Gui(), board( Board() ), window( VideoMode( 504, 504 ), "The Chess!" ), offset( 28, 28 ) {
    // RenderWindow window(VideoMode(504, 504), "The Chess!");
    // window = RenderWindow((VideoMode(504, 504), "The Chess!"));
    // ConnectToEngine("stockfish.exe");

    Texture t1, t2;
    t1.loadFromFile( "../images/figures.png" );
    t2.loadFromFile( "../images/board.png" );

    for ( int i = 0; i < 32; i++ ) f[i].setTexture( t1 );
    // Sprite sBoard(t2);
    sBoard = Sprite( t2 );

    loadPosition();
};

void WindowGui::loadPosition() {
    int k = 0;
    for ( int i = 0; i < 8; i++ )
        for ( int j = 0; j < 8; j++ ) {
            std::optional<Piece> piece = board.squares[j + i * 8];
            if ( piece.has_value() ) {
                int x = abs( piece->getValue() ) - 1;
                int y = piece->getColor() == WHITE ? 1 : 0;
                f[k].setTextureRect( IntRect( size * x, size * y, size, size ) );
                f[k].setPosition( size * j, size * i );
                k++;
            } else
                continue;
        }

    for ( int i = 0; i < (int)position.length(); i += 5 ) move( position.substr( i, 4 ) );
}

void WindowGui::_move( std::string str ) {
    Vector2f oldPos = toCoord( str[0], str[1] );
    Vector2f newPos = toCoord( str[2], str[3] );

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == newPos ) f[i].setPosition( -100, -100 );

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == oldPos ) f[i].setPosition( newPos );
}

void WindowGui::makeMove( std::string move ) {
    _move( move );  // here we need also to call makeMove from board?  bcs right now it is  only GUI
    draw();
}
void WindowGui::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ){};

Vector2f WindowGui::toCoord( char a, char b ) {
    int x = int( a ) - 97;
    int y = 7 - int( b ) + 49;
    return Vector2f( x * size, y * size );
}

void WindowGui::draw() {
    window.clear();
    window.draw( sBoard );
    for ( int i = 0; i < 32; i++ ) f[i].move( offset );
    for ( int i = 0; i < 32; i++ ) window.draw( f[i] );
    window.draw( f[n] );
    for ( int i = 0; i < 32; i++ ) f[i].move( -offset );
    window.display();
}

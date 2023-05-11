#include "Gui.hpp"

WindowGui::WindowGui( Board& b ) : window( VideoMode( 504, 504 ), "The Chess!" ), offset( 28, 28 ), board( b ) {
    // RenderWindow window(VideoMode(504, 504), "The Chess!");
    // window = RenderWindow((VideoMode(504, 504), "The Chess!"));
    // ConnectToEngine("stockfish.exe");

    // Texture t1, t2;
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
                int x = abs( piece->type ) - 1;
                int y = piece->color == WHITE ? 1 : 0;
                f[k].setTextureRect( IntRect( size * x, size * y, size, size ) );
                f[k].setPosition( size * j, size * i );
                k++;
            } else
                continue;
        }

    for ( int i = 0; i < (int)position.length(); i += 5 ) _move( position.substr( i, 4 ) );
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
    board.makeMove( move );
    // draw();
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

void WindowGui::dragAndDrop( Event e, Vector2i pos ) {
    if ( e.type == Event::MouseButtonPressed )
        if ( e.mouseButton.button == Mouse::Left )
            for ( int i = 0; i < 32; i++ )
                if ( f[i].getGlobalBounds().contains( pos.x, pos.y ) ) {
                    isMove = true;
                    n = i;
                    dx = pos.x - f[i].getPosition().x;
                    dy = pos.y - f[i].getPosition().y;
                    oldPos = f[i].getPosition();
                }

    if ( e.type == Event::MouseButtonReleased )
        if ( e.mouseButton.button == Mouse::Left ) {
            isMove = false;
            Vector2f p = f[n].getPosition() + Vector2f( size / 2, size / 2 );
            newPos = Vector2f( size * int( p.x / size ), size* int( p.y / size ) );
            str = toChessNote( oldPos ) + toChessNote( newPos );
            makeMove( str );  // wrapper to our make move
            if ( oldPos != newPos ) position += str + " ";
            f[n].setPosition( newPos );
        }
}
std::string WindowGui::toChessNote( Vector2f p ) {
    std::string s = "";
    s += char( p.x / size + 97 );
    s += char( 7 - p.y / size + 49 );
    return s;
}

void WindowGui::start() {}
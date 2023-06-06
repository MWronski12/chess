#include "Gui.hpp"

#include <string>

WindowGui::WindowGui() : engine_( Engine() ), window( VideoMode( 504, 504 ), "The Chess!" ), offset( 28, 28 ) {
    // Texture t1, t2;
    t1.loadFromFile( "../images/figures.png" );
    t2.loadFromFile( "../images/board1.png" );

    for ( int i = 0; i < 32; i++ ) f[i].setTexture( t1 );
    // Sprite sBoard(t2);
    sBoard = Sprite( t2 );

    loadPosition();
};

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

void WindowGui::loadPosition() {
    int k = 0;
    for ( int i = 0; i < 8; i++ )
        for ( int j = 0; j < 8; j++ ) {
            std::optional<Piece> piece = engine_.board.squares[j + i * 8];
            if ( piece.has_value() ) {
                int x = abs( piece->type );
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
    // w gui dodać promotion

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == newPos )
            //
            f[i].setPosition( -100, -100 );

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == oldPos )
            //
            f[i].setPosition( newPos );
}

void WindowGui::makeMove( std::string move ) {
    // Calculate and validate SquareIndex
    int src = 8 * ( 8 - ( int( move[1] ) - 48 ) ) + ( int( toupper( move[0] ) ) - 65 );
    int dest = 8 * ( 8 - ( int( move[3] ) - 48 ) ) + ( int( toupper( move[2] ) ) - 65 );
    if ( src < 0 || src > 63 || dest < 0 || dest > 63 ) {
        throw std::invalid_argument( "Invalid square notation!" );
    }

    PieceType promotion;

    // Normal move like d2d4
    if ( move.size() == 4 ) {
        makeMove( src, dest );
    }
    // Promotion move like d7d8q
    else if ( move.size() == 5 ) {
        switch ( toupper( move[4] ) ) {
            case 'Q':
                promotion = QUEEN;
                break;

            case 'R':
                promotion = ROOK;
                break;

            case 'B':
                promotion = BISHOP;
                break;

            case 'N':
                promotion = KNIGHT;
                break;

            default:
                throw std::invalid_argument( "Invalid promotion type!" );
                break;
        }
        makeMove( src, dest, promotion );

    }
    // Invalid move
    else {
        throw std::invalid_argument( "Invalid move notation!" );
    }
    // engine_.board.makeMove( move );
    // _move( move );  // here we need also to call makeMove from board?  bcs right now it is  only GUI
    // draw();
}
void WindowGui::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    auto& piece = engine_.board.squares[src];

    // Assert move is a pseudo valid move
    if ( std::find( piece->validMoves.cbegin(), piece->validMoves.cend(), dest ) == piece->validMoves.cend() ) {
        throw std::logic_error( "Invalid move!" );
    }

    // Assert move is a fully valid move
    if ( engine_.makeMove( src, dest, promotion ) == false ) {
        throw std::logic_error( "Move leaves the king in check!" );
    };
    std::string str = toChessNotation( src, dest );
    _move( str );
    // draw();
}

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

    //     std::cout << "  A  B  C  D  E  F  G  H\r\n";
    // std::cout << "Player to move: " << ( engine_.board.sideToMove == WHITE ? "White" : "Black" ) << "\r\n";
    // std::cout << "Best move: " << ( engine_.getBestMove().toString() ) << "\r\n";
    // std::cout << "Evaluation: " << ( engine_.board.score ) << "\r\n";
    // std::cout << "\n\n";
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
            newPos = Vector2f( size * int( p.x / size ), size * int( p.y / size ) );
            str = toChessNote( oldPos ) + toChessNote( newPos );

            try {
                makeMove( str );  // wrapper to our make move
            } catch ( std::exception& e ) {
                std::cout << e.what() << "\r\n";
                f[n].setPosition( oldPos );
                return;
            }
            // makeMove( str );  // wrapper to our make move
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

void WindowGui::start() {
    draw();

    // bool compFlag = false;

    while ( window.isOpen() ) {
        Vector2i pos = Mouse::getPosition( window ) - Vector2i( offset );

        Event e;
        while ( window.pollEvent( e ) ) {
            if ( e.type == Event::Closed ) {
                window.close();
                break;
            }

            // ////move back//////
            // if (e.type == Event::KeyPressed)
            //   if (e.key.code == Keyboard::BackSpace) {
            //     if (position.length() > 6)
            //       position.erase(position.length() - 6, 5);
            //     loadPosition();
            //   }

            // draw();
            // while ( true ) {
            //     std::string move;
            //     std::cout << "Enter move: ";
            //     std::cin >> move;
            //     try {
            //         makeMove( move );
            //     } catch ( std::exception& e ) {
            //         std::cout << e.what() << "\r\n";
            //     }
            // }

            /////drag and drop///////
            dragAndDrop( e, pos );
            // Search search_;
            // PieceValidMoves generator_;
            // comp
            if ( Keyboard::isKeyPressed( Keyboard::Space ) ) {
                MoveContent mov = engine_.getBestMove();
                std::string str = toChessNotation( mov.src, mov.dest );
                std::cout << str << "\r\n";
                // std::string str = engine_.getBestMove().toString();

                oldPos = toCoord( str[0], str[1] );
                newPos = toCoord( str[2], str[3] );

                for ( int i = 0; i < 32; i++ )
                    if ( f[i].getPosition() == oldPos ) n = i;

                /////animation///////
                for ( int k = 0; k < 50; k++ ) {
                    Vector2f p = newPos - oldPos;
                    f[n].move( p.x / 50, p.y / 50 );
                    window.draw( sBoard );
                    for ( int i = 0; i < 32; i++ ) f[i].move( offset );
                    for ( int i = 0; i < 32; i++ ) window.draw( f[i] );
                    window.draw( f[n] );
                    for ( int i = 0; i < 32; i++ ) f[i].move( -offset );
                    window.display();
                }

                makeMove( str );
                position += str + " ";
                f[n].setPosition( newPos );
                // std::cout << "Player to move: " << ( engine_.board.sideToMove == WHITE ? "White" : "Black" ) <<
                // "\r\n"; std::cout << "Best move: " << ( engine_.getBestMove().toString() ) << "\r\n"; std::cout <<
                // "Evaluation: " << ( engine_.board.score ) << "\r\n"; std::cout << "\n\n";
            }
        }

        if ( isMove )
            f[n].setPosition( pos.x - dx, pos.y - dy );  // if we are moving piece, we need to update its position

        draw();
    }
}
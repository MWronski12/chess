#include "Gui.hpp"

#include <string>

WindowGui::WindowGui() : engine_( Engine() ), window_( VideoMode( 504, 504 ), "The Chess!" ), offset_( 28, 28 ) {
    figuresImages_.loadFromFile( "../images/figures.png" );
    boardImage_.loadFromFile( "../images/board1.png" );

    for ( int i = 0; i < 32; i++ ) f[i].setTexture( figuresImages_ );
    board_ = Sprite( boardImage_ );

    loadPosition();
};

void WindowGui::loadPosition() {
    int k = 0;
    for ( int i = 0; i < 8; i++ )
        for ( int j = 0; j < 8; j++ ) {
            std::optional<Piece> piece = engine_.board.squares[j + i * 8];
            if ( piece.has_value() ) {
                int x = abs( piece->type );
                int y = piece->color == WHITE ? 1 : 0;
                f[k].setTextureRect( IntRect( FIGURE_SIZE * x, FIGURE_SIZE * y, FIGURE_SIZE, FIGURE_SIZE ) );
                f[k].setPosition( FIGURE_SIZE * j, FIGURE_SIZE * i );
                k++;
            } else
                continue;
        }

    for ( int i = 0; i < (int)strPosition_.length(); i += 5 ) makeMoveOnGui( strPosition_.substr( i, 4 ) );
}

void WindowGui::start() {
    draw();

    while ( window_.isOpen() ) {
        Vector2i pos = Mouse::getPosition( window_ ) - Vector2i( offset_ );

        Event e;
        while ( window_.pollEvent( e ) ) {
            if ( e.type == Event::Closed ) {
                window_.close();
                break;
            }

            dragAndDrop( e, pos );

            if ( Keyboard::isKeyPressed( Keyboard::Space ) ) {
                MoveContent mov = engine_.getBestMove();
                std::string str = moveContentToChessNotation( mov.src, mov.dest );

                oldPos_ = toSquareCoord( str[0], str[1] );
                newPos_ = toSquareCoord( str[2], str[3] );

                for ( int i = 0; i < 32; i++ )
                    if ( f[i].getPosition() == oldPos_ ) currentFigureIndex_ = i;

                /////animation///////
                for ( int k = 0; k < 50; k++ ) {
                    Vector2f p = newPos_ - oldPos_;
                    f[currentFigureIndex_].move( p.x / 50, p.y / 50 );
                    window_.draw( board_ );
                    for ( int i = 0; i < 32; i++ ) f[i].move( offset_ );
                    for ( int i = 0; i < 32; i++ ) window_.draw( f[i] );
                    window_.draw( f[currentFigureIndex_] );
                    for ( int i = 0; i < 32; i++ ) f[i].move( -offset_ );
                    window_.display();
                }

                makeMove( str );
                strPosition_ += str + " ";
                f[currentFigureIndex_].setPosition( newPos_ );
                std::cout << "Evaluation: " << ( engine_.board.score ) << "\r\n";
            }
        }

        // if we are moving piece, we need to update its position in GUI
        if ( isMove_ ) f[currentFigureIndex_].setPosition( pos.x - dx_, pos.y - dy_ );

        draw();
    }
}

std::string WindowGui::squareToChessNotation( int square ) {
    int rank = 8 - square / 8;
    int file = square % 8;
    std::string notation = "";
    notation += char( file + 'a' );
    notation += char( rank + '0' );
    return notation;
}

std::string WindowGui::moveContentToChessNotation( int src, int dest ) {
    std::string srcNotation = squareToChessNotation( src );
    std::string destNotation = squareToChessNotation( dest );
    return srcNotation + destNotation;
}

std::string WindowGui::vector2ToChessNotation( Vector2f p ) {
    std::string str = "";
    str += char( p.x / FIGURE_SIZE + 97 );
    str += char( 7 - p.y / FIGURE_SIZE + 49 );
    return str;
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
    std::string str = moveContentToChessNotation( src, dest );
    makeMoveOnGui( str );
}

void WindowGui::makeMoveOnGui( std::string str ) {
    Vector2f oldPos_ = toSquareCoord( str[0], str[1] );
    Vector2f newPos_ = toSquareCoord( str[2], str[3] );

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == newPos_ ) f[i].setPosition( -100, -100 );

    for ( int i = 0; i < 32; i++ )
        if ( f[i].getPosition() == oldPos_ ) f[i].setPosition( newPos_ );
}

Vector2f WindowGui::toSquareCoord( char a, char b ) {
    int x = int( a ) - 97;
    int y = 7 - int( b ) + 49;
    return Vector2f( x * FIGURE_SIZE, y * FIGURE_SIZE );
}

void WindowGui::draw() {
    window_.clear();
    window_.draw( board_ );
    for ( int i = 0; i < 32; i++ ) f[i].move( offset_ );
    for ( int i = 0; i < 32; i++ ) window_.draw( f[i] );
    window_.draw( f[currentFigureIndex_] );
    for ( int i = 0; i < 32; i++ ) f[i].move( -offset_ );
    window_.display();
}

void WindowGui::dragAndDrop( Event e, Vector2i pos ) {
    if ( e.type == Event::MouseButtonPressed )
        if ( e.mouseButton.button == Mouse::Left )
            for ( int i = 0; i < 32; i++ )
                if ( f[i].getGlobalBounds().contains( pos.x, pos.y ) ) {
                    isMove_ = true;
                    currentFigureIndex_ = i;
                    dx_ = pos.x - f[i].getPosition().x;
                    dy_ = pos.y - f[i].getPosition().y;
                    oldPos_ = f[i].getPosition();
                }

    if ( e.type == Event::MouseButtonReleased )
        if ( e.mouseButton.button == Mouse::Left ) {
            isMove_ = false;
            Vector2f p = f[currentFigureIndex_].getPosition() + Vector2f( FIGURE_SIZE / 2, FIGURE_SIZE / 2 );
            newPos_ = Vector2f( FIGURE_SIZE * int( p.x / FIGURE_SIZE ), FIGURE_SIZE * int( p.y / FIGURE_SIZE ) );
            std::string str = vector2ToChessNotation( oldPos_ ) + vector2ToChessNotation( newPos_ );

            try {
                makeMove( str );
            } catch ( std::exception& e ) {
                std::cout << e.what() << "\r\n";
                f[currentFigureIndex_].setPosition( oldPos_ );
                return;
            }
            if ( oldPos_ != newPos_ ) strPosition_ += str + " ";
            f[currentFigureIndex_].setPosition( newPos_ );
        }
}

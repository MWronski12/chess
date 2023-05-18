#include <inttypes.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <exception>
#include <iostream>

#include "Board.h"
#include "Engine.h"
#include "Piece.h"

class Gui {
public:
    Gui() = default;
    Gui( const Gui& ) = delete;             // Delete copy constructor
    Gui& operator=( const Gui& ) = delete;  // Delete copy assignment operator
    //
    // By SquareIndex - upper left corner is 0 and bottom right corner is 63
    virtual void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY ) = 0;

    // By d2d4 notation
    virtual void makeMove( std::string move ) = 0;

    virtual void start() = 0;
    virtual void draw() {}
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                      WindowGui                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

using namespace sf;

// Vector2f offset(28, 28);

class WindowGui : public Gui {
public:
    WindowGui( Board& board );
    virtual void makeMove( std::string move ) override;
    virtual void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) override;
    virtual void draw() override;
    void start() override;
    void dragAndDrop( Event e, Vector2i pos );
    std::string toChessNote( Vector2f p );
    RenderWindow window;   // ok
    Vector2f offset;       // ok
    Sprite f[32];          // ok
    bool isMove = false;   // ok
    float dx = 0, dy = 0;  // ok
    int n = 0;             // ok

private:
    Board& board;    // ok
    Texture t1, t2;  // ok
    Sprite sBoard;
    Vector2f oldPos, newPos;    // ok
    std::string str;            // ok
    std::string position = "";  // ok
    int size = 56;              // ok
    void loadPosition();
    void _move( std::string str );
    Vector2f toCoord( char a, char b );
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                     Console Gui                                                    */
/* ------------------------------------------------------------------------------------------------------------------ */
class ConsoleGui : public Gui {
public:
    ConsoleGui() : engine_( Engine() ) {}

    void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY ) override {
        auto& piece = engine_.board.squares[src];

        // Assert move is a pseudo valid move
        if ( std::find( piece->validMoves.cbegin(), piece->validMoves.cend(), dest ) == piece->validMoves.cend() ) {
            throw std::logic_error( "Invalid move!" );
        }

        // Assert move is a fully valid move
        if ( engine_.makeMove( src, dest, promotion ) == false ) {
            throw std::logic_error( "Move leaves the king in check!" );
        };

        draw();
    }

    void makeMove( std::string move ) override {
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

    void start() {
        draw();
        while ( true ) {
            std::string move;
            std::cout << "Enter move: ";
            std::cin >> move;
            try {
                makeMove( move );
            } catch ( std::exception& e ) {
                std::cout << e.what() << "\r\n";
            }
        }
    }

private:
    Engine engine_;

    char pieceToChar( PieceColor color, PieceType type ) const {
        char result;

        switch ( type ) {
            case KING:
                result = 'k';
                break;

            case QUEEN:
                result = 'q';
                break;

            case ROOK:
                result = 'r';
                break;

            case BISHOP:
                result = 'b';
                break;

            case KNIGHT:
                result = 'n';
                break;

            case PAWN:
                result = 'p';
                break;

            default:
                throw std::logic_error( "Invalid piece type!" );
        }

        // Capitalize for white piece
        if ( color == WHITE ) {
            result = toupper( result );
        }

        return result;
    }

    void draw() const {
        for ( int i = 0; i < 64; i++ ) {
            if ( i % 8 == 0 ) {
                std::cout << ( 64 - i ) / 8 << " ";
            }
            if ( engine_.board.squares[i] ) {
                std::cout << pieceToChar( engine_.board.squares[i]->color, engine_.board.squares[i]->type );
            } else {
                std::cout << '-';
            }
            if ( i % 8 == 7 ) {
                std::cout << "\r\n";
            } else {
                std::cout << ", ";
            }
        }

        std::cout << "  A  B  C  D  E  F  G  H\r\n";
        std::cout << "Player to move: " << ( engine_.board.sideToMove == WHITE ? "White" : "Black" ) << "\r\n";
        std::cout << "Best move: " << ( engine_.getBestMove() ) << "\r\n";
        std::cout << "Evaluation: " << ( engine_.getEvaluation() ) << "\r\n";
        std::cout << "\n\n";
    }
};

#ifndef GUI_H
#define GUI_H

#include <inttypes.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>

#include "Board.h"
#include "Piece.h"


class Gui {
public:
    Gui() = default;
    Gui( const Gui & ) = delete;             // Delete copy constructor
    Gui &operator=( const Gui & ) = delete;  // Delete copy assignment operator
    //
    // By SquareIndex - upper left corner is 0 and bottom right corner is 63
    virtual void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) = 0;

    // By d2d4 notation
    virtual void makeMove( std::string move ) = 0;
    // ej trzeba dziedziczyć wszystkie metody?
    virtual void draw() = 0;
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                      WindowGui                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

using namespace sf;

// Vector2f offset(28, 28);

class WindowGui : public Gui {
public:
    WindowGui();
    virtual void makeMove( std::string move ) override;
    virtual void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) override;
    virtual void draw() override;

private:
    Board board;
    RenderWindow window;
    Vector2f offset;
    int size = 56;
    Sprite f[32];  // figures
    std::string position = "";
    int n = 0;
    Sprite sBoard;
    void loadPosition();
    void _move( std::string str );
    Vector2f toCoord( char a, char b );
};

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                     ConsoleGui                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

// class ConsoleGui : public Gui {
// public:
//     ConsoleGui() : board( Board() ) { draw(); }
//     void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) override {
//         board.makeMove( src, dest, promotion );
//         draw();
//     }

//     void makeMove( std::string move ) override {
//         board.makeMove( move );
//         draw();
//     }

// private:
//     Board board;

//     char pieceToChar( PieceColor color, PieceType type ) const {
//         char result;

//         switch ( type ) {
//             case KING:
//                 result = 'k';
//                 break;

//             case QUEEN:
//                 result = 'q';
//                 break;

//             case ROOK:
//                 result = 'r';
//                 break;

//             case BISHOP:
//                 result = 'b';
//                 break;

//             case KNIGHT:
//                 result = 'n';
//                 break;

//             case PAWN:
//                 result = 'p';
//                 break;

//             default:
//                 throw std::logic_error( "Invalid piece type!" );
//         }

//         // Capitalize for white piece
//         if ( color == WHITE ) {
//             result = toupper( result );
//         }

//         return result;
//     }

//     void draw() const {
//         std::cout << "\n\n";

//         for ( int i = 0; i < 64; i++ ) {
//             if ( i % 8 == 0 ) {
//                 std::cout << ( 64 - i ) / 8 << " ";
//             }
//             if ( board.squares[i] ) {
//                 std::cout << pieceToChar( board.squares[i]->getColor(), board.squares[i]->type );
//             } else {
//                 std::cout << '-';
//             }
//             if ( i % 8 == 7 ) {
//                 std::cout << "\r\n";
//             } else {
//                 std::cout << ", ";
//             }
//         }

//         std::cout << "  A  B  C  D  E  F  G  H\r\n";
//     }
// };

#endif
#include <algorithm>
#include <exception>
#include <iostream>

#include "Engine.h"

class Gui {
public:
    // By SquareIndex - upper left corner is 0 and bottom right corner is 63
    virtual void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY ) = 0;

    // By d2d4 notation
    virtual void makeMove( std::string move ) = 0;

    virtual void start() = 0;
};

class ConsoleGui : public Gui {
public:
    ConsoleGui() : engine( Engine() ) {}
    void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY ) override {
        auto& piece = engine.board->squares[src];

        // Assert move is a pseudo valid move
        if ( std::find( piece->validMoves.cbegin(), piece->validMoves.cend(), dest ) == piece->validMoves.cend() ) {
            throw std::logic_error( "Invalid move!" );
        }

        // Assert move is a fully valid move
        if ( engine.makeMove( src, dest, promotion ) == false ) {
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
    Engine engine;

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
        std::cout << "\n\n";

        for ( int i = 0; i < 64; i++ ) {
            if ( i % 8 == 0 ) {
                std::cout << ( 64 - i ) / 8 << " ";
            }
            if ( engine.board->squares[i] ) {
                std::cout << pieceToChar( engine.board->squares[i]->getColor(), engine.board->squares[i]->type );
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
    }
};

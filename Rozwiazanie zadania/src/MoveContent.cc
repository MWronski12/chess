/**
 * @file MoveContent.cc
 * @author Mikołaj Wroński
 * @brief Implementation of MoveContent class. Represents a single move.
 * @date 2023-06-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "MoveContent.h"

MoveContent::MoveContent( SquareIndex src, SquareIndex dest, PieceType promotion, PieceType pieceMoving,
                          PieceType pieceTaken, bool isEnPassantCapture, int score )
    : src( src ),
      dest( dest ),
      promotion( promotion ),
      pieceMoving( pieceMoving ),
      pieceTaken( pieceTaken ),
      isEnPassantCapture( isEnPassantCapture ),
      score( score ) {}

MoveContent::MoveContent()
    : src( NULL_SQUARE ),
      dest( NULL_SQUARE ),
      promotion( EMPTY ),
      pieceMoving( EMPTY ),
      pieceTaken( EMPTY ),
      isEnPassantCapture( false ),
      score( 0 ) {}

bool MoveContent::operator==( const MoveContent& other ) const {
    return src == other.src && dest == other.dest && promotion == other.promotion;
}

std::string MoveContent::toString() const {
    std::string str = "";
    str += (char)( 'a' + ( src % 8 ) );
    str += (char)( '1' + ( 7 - src / 8 ) );
    str += (char)( 'a' + ( dest % 8 ) );
    str += (char)( '1' + ( 7 - dest / 8 ) );
    if ( promotion != EMPTY ) {
        str += (char)( 'a' + ( promotion % 8 ) );
        str += (char)( '1' + ( promotion / 8 ) );
    }
    return str;
}

int MoveContent::compareMin( const MoveContent& m1, const MoveContent& m2 ) { return m1.score < m2.score; }
int MoveContent::compareMax( const MoveContent& m1, const MoveContent& m2 ) { return m1.score > m2.score; }

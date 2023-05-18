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

// TODO
std::string MoveContent::toPGN() const { return "O-O-O"; }
std::string MoveContent::toSAN() const {
    std::string san = "";
    san += (char)( 'a' + ( src % 8 ) );
    san += (char)( '1' + ( 7 - src / 8 ) );
    san += (char)( 'a' + ( dest % 8 ) );
    san += (char)( '1' + ( 7 - dest / 8 ) );
    if ( promotion != EMPTY ) {
        san += (char)( 'a' + ( promotion % 8 ) );
        san += (char)( '1' + ( promotion / 8 ) );
    }
    return san;
}

int MoveContent::compareMin( const MoveContent& m1, const MoveContent& m2 ) { return m1.score < m2.score; }
int MoveContent::compareMax( const MoveContent& m1, const MoveContent& m2 ) { return m1.score > m2.score; }

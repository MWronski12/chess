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

// TODO
std::string MoveContent::toPGN() const { return "O-O-O"; }

int MoveContent::compare( const MoveContent& m1, const MoveContent& m2 ) { return m1.score < m2.score; }

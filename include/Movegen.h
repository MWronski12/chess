#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>
#include <memory>
#include <vector>

#include "Board.h"
#include "PieceMoves.h"

class PieceValidMoves {
public:
    PieceValidMoves();
    PieceValidMoves( PieceValidMoves & ) = delete;
    PieceValidMoves( PieceValidMoves && ) = delete;

    // Generate all pseudo legal moves for the given board filling Piece.validMoves vectors
    int generateValidMoves( Board &board );

    // This method looks at the board and determines if previously made move didnt leave the king in check.
    // All pseudo legal moves generated with generateValidMoves method have to be validated for full legality!
    bool validateBoard( const Board &board ) const;

private:
    // Kings and pawns have different restrictions on moves so they are handled separately
    int generateValidKingMoves( Board &board, SquareIndex srcSquare );
    int generateValidCastlingMoves( Board &board, SquareIndex srcSquare );
    int generateValidPawnMoves( Board &board, SquareIndex srcSquare );

    // Analyze methods will record information about the board while looking at the move
    // They will return true if the move is valid
    bool analyzeMove( Board &board, SquareIndex srcSquare, SquareIndex dest );
    bool analyzePawnMove( Board &board, SquareIndex srcSquare, SquareIndex destSquare );
    bool analyzeCastlingMove( Board &board, SquareIndex srcSquare, SquareIndex destSquare );

    // Only used for castling moves
    std::array<bool, 64> blackAttackBoard_;
    std::array<bool, 64> whiteAttackBoard_;

    // Track the king's position to analyze at the end
    SquareIndex blackKingSquare_;
    SquareIndex whiteKingSquare_;

    PieceMoves &pieceMoves_;
};

#endif

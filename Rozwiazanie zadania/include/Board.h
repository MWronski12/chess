#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <optional>

#include "Common.h"
#include "MoveContent.h"
#include "Piece.h"

class Board {
public:
    // constructors
    Board();
    Board( std::string FENString );
    Board fastCopy() const;

    // members
    bool whiteIsChecked;
    bool blackIsChecked;
    bool whiteHasCastled;
    bool blackHasCastled;
    int score;
    GameResult gameResult;
    PieceColor sideToMove;
    MoveContent lastMove;
    SquareIndex enPassantSquare;
    std::array<std::optional<Piece>, 64> squares;

    // methods
    void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY );
    void makeMove( std::string move );  // d2d4 notation (d7d8Q for promotion)

    std::string toFEN() const;

private:
    int fullMoveCounter_;
    int fiftyMoveCounter_;
    int threefoldRepetitionCounter_;

    // Helper methods
    bool enPassantIsAvailable() const;
    void validateMove( SquareIndex src, SquareIndex dest, PieceType promotion ) const;
    void recordEnPassant( SquareIndex src, SquareIndex dest );
    void handleEnPassant();
    void handleCastling( SquareIndex src, SquareIndex dest );
    void handlePromotion( SquareIndex src, PieceType promotion );

    // Castling rights methods
    bool whiteCanCastle() const;
    bool whiteCanCastleKingSide() const;
    bool whiteCanCastleQueenSide() const;
    bool blackCanCastle() const;
    bool blackCanCastleKingSide() const;
    bool blackCanCastleQueenSide() const;
};

#endif
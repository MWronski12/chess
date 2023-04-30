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
    bool whiteIsCheckMated;
    bool blackIsCheckMated;
    bool staleMate;
    int score;
    PieceColor sideToMove;
    MoveContent lastMove;
    SquareIndex enPassantSquare;
    std::array<std::optional<Piece>, 64> squares;

    uint64_t piece_keys[12][64];
    uint64_t side_key;
    uint64_t castling_keys[4];
    uint64_t hash_key;
    uint64_t getHashKey();
    uint64_t updateHashKey( uint64_t hash_key, const MoveContent& move );
    void initZobrist();

    // methods
    void makeMove( SquareIndex src, SquareIndex dest, PieceType promotion = EMPTY );
    void makeMove( std::string move );  // d2d4 notation (d7d8Q for promotion)
    // TODO: serializes Board object to FEN string notation
    std::string toFEN() const;

private:
    int fiftyMoveCounter_;
    int threefoldRepetitionCounter_;

    // Helper methods
    bool enPassantIsAvailable() const;
    void validateMove( SquareIndex src, SquareIndex dest, PieceType promotion ) const;
    void recordEnPassant( SquareIndex src, SquareIndex dest );
    void handleEnPassant();
    void handleCastling( SquareIndex src, SquareIndex dest );
    void handlePromotion( SquareIndex src, PieceType promotion );
};

#endif

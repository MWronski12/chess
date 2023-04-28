#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <array>
#include <vector>

#include "Common.h"
#include "Piece.h"

class PieceMoves {
public:
    static PieceMoves &getInstance() {
        static PieceMoves instance;
        return instance;
    }

    const std::vector<std::vector<SquareIndex>> &getMoveList( PieceColor color, PieceType piece,
                                                              SquareIndex square ) const;

private:
    PieceMoves();
    PieceMoves( PieceMoves &other ) = delete;

    std::array<std::vector<std::vector<SquareIndex>>, 64> whitePawnMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> blackPawnMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> knightMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> bishopMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> rookMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> queenMoves_;
    std::array<std::vector<std::vector<SquareIndex>>, 64> kingMoves_;

    void generateWhitePawnMoves();
    void generateBlackPawnMoves();
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateQueenMoves();
    void generateKingMoves();
};

#endif
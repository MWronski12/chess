#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <array>
#include <vector>

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
    // Private constructor to prevent creation of instances from outside the class.
    PieceMoves();

    std::array<std::vector<std::vector<SquareIndex>>, 64> _whitePawnMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _blackPawnMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _knightMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _bishopMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _rookMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _queenMoves;
    std::array<std::vector<std::vector<SquareIndex>>, 64> _kingMoves;

    void generateWhitePawnMoves();
    void generateBlackPawnMoves();
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateQueenMoves();
    void generateKingMoves();
};

#endif
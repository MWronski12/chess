#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include "Board.h"
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

class PieceValidMoves {
public:
    PieceValidMoves();

    // Generate all valid moves for the given board filling Piece.validMoves vectors
    int generateValidMoves( Board &board );

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
    std::array<bool, 64> _blackAttackBoard;
    std::array<bool, 64> _whiteAttackBoard;

    // Track the king's position to analyze at the end
    SquareIndex _blackKingSquare;
    SquareIndex _whiteKingSquare;

    PieceMoves &_pieceMoves;
};

#endif

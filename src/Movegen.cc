#include "Movegen.h"

/* -------------------------------------------------------------------------- */
/*                              Pieve Valid Moves                             */
/* -------------------------------------------------------------------------- */

PieceValidMoves::PieceValidMoves()
    : blackAttackBoard_{ false },
      whiteAttackBoard_{ false },
      blackKingSquare_( NULL_SQUARE ),
      whiteKingSquare_( NULL_SQUARE ),
      pieceMoves_( PieceMoves::getInstance() ) {}

// Generate valid moves for every piece on the board filling Piece.validMoves vectors
int PieceValidMoves::generateValidMoves( Board& board ) {
    int movesGeneratedCount = 0;

    // Reset attack boards
    blackAttackBoard_.fill( false );
    whiteAttackBoard_.fill( false );

    // Reset king position
    blackKingSquare_ = NULL_SQUARE;
    whiteKingSquare_ = NULL_SQUARE;

    // Reset checks
    board.blackIsChecked = false;
    board.whiteIsChecked = false;

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto& piece = board.squares[srcSquare];
        if ( piece ) {
            // TODO:
            // manually resize to the count of previously generated moves to avoid dynamic resizing
            // requires adding piece.lastValidMovesCount to the Piece class
            // Clear the previous valid moves
            piece->validMoves.clear();
            piece->attackedValue = 0;
            piece->defendedValue = 0;

            // Kings need information about which squares are attacked,
            // which we gather during analysis, so we just record the position to analyze later
            if ( piece->type == KING ) {
                piece->color == WHITE ? whiteKingSquare_ = srcSquare : blackKingSquare_ = srcSquare;
                continue;
            }

            // Pawns behave different than other pieces so we analyze their moves separately
            if ( piece->type == PAWN ) {
                movesGeneratedCount += generateValidPawnMoves( board, srcSquare );
                continue;
            }

            // For all other pieces we iterate through all the rays and each move to generate valid moves
            for ( auto& ray : pieceMoves_.getMoveList( piece->color, piece->type, srcSquare ) ) {
                for ( auto destSquare : ray ) {
                    // Analyze the move to gather information about the board
                    // Analyze move method will return true if the move is valid
                    if ( analyzeMove( board, srcSquare, destSquare ) ) {
                        piece->validMoves.push_back( destSquare );
                        movesGeneratedCount++;
                    }
                    // We cannot continue passed a piece, so we can skip to the next ray
                    if ( board.squares[destSquare] ) {
                        break;
                    }
                }
            }
        }
    }

    // Now that we have gathered information about the board we can generate the kings moves
    if ( board.sideToMove == WHITE ) {
        movesGeneratedCount += generateValidKingMoves( board, blackKingSquare_ );
        movesGeneratedCount += generateValidKingMoves( board, whiteKingSquare_ );
    } else {
        movesGeneratedCount += generateValidKingMoves( board, whiteKingSquare_ );
        movesGeneratedCount += generateValidKingMoves( board, blackKingSquare_ );
    }

    generateValidCastlingMoves( board, whiteKingSquare_ );
    generateValidCastlingMoves( board, blackKingSquare_ );

    return movesGeneratedCount;
}

bool PieceValidMoves::validateBoard( const Board& board ) const {
    if ( board.sideToMove == WHITE && board.blackIsChecked ) {
        return false;
    } else if ( board.sideToMove == BLACK && board.whiteIsChecked ) {
        return false;
    } else {
        return true;
    }
}

int PieceValidMoves::generateValidPawnMoves( Board& board, SquareIndex srcSquare ) {
    int movesGeneratedCount = 0;
    auto& pawn = board.squares[srcSquare];

    for ( auto& ray : pieceMoves_.getMoveList( pawn->color, PAWN, srcSquare ) ) {
        for ( auto destSquare : ray ) {
            if ( analyzePawnMove( board, srcSquare, destSquare ) ) {
                pawn->validMoves.push_back( destSquare );
                movesGeneratedCount++;
            }
            // We cannot continue passed a piece, so we can skip to the next ray
            if ( board.squares[destSquare] ) {
                break;
            }
        }
    }

    return movesGeneratedCount;
}

int PieceValidMoves::generateValidKingMoves( Board& board, SquareIndex srcSquare ) {
    int movesGeneratedCount = 0;
    auto& king = board.squares[srcSquare];

    // Iterate through kings moves
    for ( auto& ray : pieceMoves_.getMoveList( king->color, KING, srcSquare ) ) {
        for ( auto destSquare : ray ) {
            // Castling moves have to be generated last so skip for now
            if ( abs( destSquare - srcSquare ) == 2 ) {
                continue;
            }
            // Normal move
            else if ( analyzeMove( board, srcSquare, destSquare ) ) {
                king->validMoves.push_back( destSquare );
                movesGeneratedCount++;
            }
        }
    }

    return movesGeneratedCount;
}

int PieceValidMoves::generateValidCastlingMoves( Board& board, SquareIndex srcSquare ) {
    int movesGeneratedCount = 0;
    auto& king = board.squares[srcSquare];

    // Black castle
    if ( srcSquare == 4 ) {
        // Kingside
        if ( analyzeCastlingMove( board, srcSquare, 6 ) ) {
            king->validMoves.push_back( 6 );
            movesGeneratedCount++;
        }
        // Queenside
        if ( analyzeCastlingMove( board, srcSquare, 2 ) ) {
            king->validMoves.push_back( 2 );
            movesGeneratedCount++;
        }
    }

    // White castle
    else if ( srcSquare == 60 ) {
        // Kingside
        if ( analyzeCastlingMove( board, srcSquare, 62 ) ) {
            king->validMoves.push_back( 62 );
            movesGeneratedCount++;
        }
        // Queenside
        if ( analyzeCastlingMove( board, srcSquare, 58 ) ) {
            king->validMoves.push_back( 58 );
            movesGeneratedCount++;
        }
    }

    return movesGeneratedCount;
}

bool PieceValidMoves::analyzeMove( Board& board, SquareIndex src, SquareIndex dest ) {
    auto& pieceMoving = board.squares[src];
    auto& pieceAttacked = board.squares[dest];

    // For all other pieces than pawns we attack every field where we can move
    // Pawns are analyzed by analyzePawnMove method
    pieceMoving->color == WHITE ? whiteAttackBoard_[dest] = true : blackAttackBoard_[dest] = true;

    // Destination square is occupied
    if ( pieceAttacked ) {
        // By allied piece
        if ( pieceAttacked->color == pieceMoving->color ) {
            pieceAttacked->defendedValue += pieceMoving->actionValue;
            return false;
        }
        // By enemy king
        else if ( pieceAttacked->type == KING ) {
            pieceAttacked->color == WHITE ? board.whiteIsChecked = true : board.blackIsChecked = true;
            return false;
        }
        // By normal enemy piece
        else {
            pieceAttacked->attackedValue += pieceMoving->actionValue;
            return true;
        }
    }

    // Destination square is empty
    return true;
}

bool PieceValidMoves::analyzePawnMove( Board& board, SquareIndex srcSquare, SquareIndex destSquare ) {
    auto& pawnMoving = board.squares[srcSquare];
    auto& pieceAttacked = board.squares[destSquare];

    // We attack the field if the pawn moves in diagonal
    if ( abs( destSquare - srcSquare ) % 8 != 0 ) {
        pawnMoving->color == WHITE ? whiteAttackBoard_[destSquare] = true : blackAttackBoard_[destSquare] = true;
    }

    /* ------------------------------- En passant ------------------------------- */
    if ( abs( destSquare - srcSquare ) % 8 != 0 && destSquare == board.enPassantSquare ) {
        return true;
    }
    /* ---------------------------- Diagonal capture ---------------------------- */
    else if ( abs( destSquare - srcSquare ) % 8 != 0 ) {
        // Destination square is occupied
        if ( pieceAttacked ) {
            // By allied piece
            if ( pieceAttacked->color == pawnMoving->color ) {
                pieceAttacked->defendedValue += pawnMoving->actionValue;
                return false;
            }
            // By enemy king
            else if ( pieceAttacked->type == KING ) {
                pieceAttacked->color == WHITE ? board.whiteIsChecked = true : board.blackIsChecked = true;
                return false;
            }
            // By normal enemy piece
            else {
                pieceAttacked->attackedValue += pawnMoving->actionValue;
                return true;
            }
        }
        // Destination square is empty - diagonal capture invalid
        else {
            return false;
        }
    }
    /* --------------------------- Normal forward move -------------------------- */
    else {
        // Pawns can move forward only if the destination square is empty
        if ( pieceAttacked ) {
            return false;
        }
        return true;
    }

    throw std::logic_error( "End of analyzePawnMove method reached without returning a value!" );
}

bool PieceValidMoves::analyzeCastlingMove( Board& board, SquareIndex srcSquare, SquareIndex destSquare ) {
    // White king side castle
    if ( srcSquare == 60 && destSquare == 62 ) {
        if ( !board.whiteCanCastleKingSide() ) return false;
        if ( blackAttackBoard_[60] || blackAttackBoard_[61] || blackAttackBoard_[62] ) return false;
        return true;
    }
    // White queen side castle
    if ( srcSquare == 60 && destSquare == 58 ) {
        if ( !board.whiteCanCastleQueenSide() ) return false;
        if ( blackAttackBoard_[58] || blackAttackBoard_[59] || blackAttackBoard_[60] ) return false;
        return true;
    }
    // Black king side castle
    if ( srcSquare == 4 && destSquare == 6 ) {
        if ( !board.blackCanCastleKingSide() ) return false;
        if ( whiteAttackBoard_[4] || whiteAttackBoard_[5] || whiteAttackBoard_[6] ) return false;
        return true;
    }
    //  Black queen side castle
    if ( srcSquare == 4 && destSquare == 2 ) {
        if ( !board.blackCanCastleQueenSide() ) return false;
        if ( whiteAttackBoard_[2] || whiteAttackBoard_[3] || whiteAttackBoard_[4] ) return false;
        return true;
    }
    throw std::logic_error( "Analyze castling move was called with a move that doesn't represent castling!" );
}

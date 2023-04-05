#include "Movegen.h"

/* -------------------------------------------------------------------------- */
/*                              Pieve Valid Moves                             */
/* -------------------------------------------------------------------------- */

PieceValidMoves::PieceValidMoves()
    : _blackAttackBoard{ false },
      _whiteAttackBoard{ false },
      _blackKingSquare( NULL_SQUARE ),
      _whiteKingSquare( NULL_SQUARE ),
      _pieceMoves( PieceMoves::getInstance() ) {}

// Generate valid moves for every piece on the board filling Piece.validMoves vectors
int PieceValidMoves::generateValidMoves( Board& board ) {
    int movesGeneratedCount = 0;

    // Reset attack boards
    _blackAttackBoard.fill( false );
    _whiteAttackBoard.fill( false );

    // Reset king position
    _blackKingSquare = NULL_SQUARE;
    _whiteKingSquare = NULL_SQUARE;

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
                piece->getColor() == WHITE ? _whiteKingSquare = srcSquare : _blackKingSquare = srcSquare;
                continue;
            }

            // Pawns behave different than other pieces so we analyze their moves separately
            if ( piece->type == PAWN ) {
                movesGeneratedCount += generateValidPawnMoves( board, srcSquare );
                continue;
            }

            // For all other pieces we iterate through all the rays and each move to generate valid moves
            for ( auto& ray : _pieceMoves.getMoveList( piece->getColor(), piece->type, srcSquare ) ) {
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
        movesGeneratedCount += generateValidKingMoves( board, _blackKingSquare );
        movesGeneratedCount += generateValidKingMoves( board, _whiteKingSquare );
    } else {
        movesGeneratedCount += generateValidKingMoves( board, _whiteKingSquare );
        movesGeneratedCount += generateValidKingMoves( board, _blackKingSquare );
    }

    generateValidCastlingMoves( board, _whiteKingSquare );
    generateValidCastlingMoves( board, _blackKingSquare );

    return movesGeneratedCount;
}

bool PieceValidMoves::validateMove( const Board& board ) const {
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

    for ( auto& ray : _pieceMoves.getMoveList( pawn->getColor(), PAWN, srcSquare ) ) {
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
    for ( auto& ray : _pieceMoves.getMoveList( king->getColor(), KING, srcSquare ) ) {
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
    pieceMoving->getColor() == WHITE ? _whiteAttackBoard[dest] = true : _blackAttackBoard[dest] = true;

    // Destination square is occupied
    if ( pieceAttacked ) {
        // By allied piece
        if ( pieceAttacked->getColor() == pieceMoving->getColor() ) {
            pieceAttacked->defendedValue += pieceMoving->getActionValue();
            return false;
        }
        // By enemy king
        else if ( pieceAttacked->type == KING ) {
            pieceAttacked->getColor() == WHITE ? board.whiteIsChecked = true : board.blackIsChecked = true;
            return false;
        }
        // By normal enemy piece
        else {
            pieceAttacked->attackedValue += pieceMoving->getActionValue();
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
        pawnMoving->getColor() == WHITE ? _whiteAttackBoard[destSquare] = true : _blackAttackBoard[destSquare] = true;
    }

    /* ------------------------------- En passant ------------------------------- */
    if ( abs( destSquare - srcSquare ) % 8 != 0 && destSquare == board.getEnPassantSquare() ) {
        return true;
    }
    /* ---------------------------- Diagonal capture ---------------------------- */
    else if ( abs( destSquare - srcSquare ) % 8 != 0 ) {
        // Destination square is occupied
        if ( pieceAttacked ) {
            // By allied piece
            if ( pieceAttacked->getColor() == pawnMoving->getColor() ) {
                pieceAttacked->defendedValue += pawnMoving->getActionValue();
                return false;
            }
            // By enemy king
            else if ( pieceAttacked->type == KING ) {
                pieceAttacked->getColor() == WHITE ? board.whiteIsChecked = true : board.blackIsChecked = true;
                return false;
            }
            // By normal enemy piece
            else {
                pieceAttacked->attackedValue += pawnMoving->getActionValue();
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
    auto& king = board.squares[srcSquare];

    /* ------------------------- Black king side castle ------------------------- */
    if ( srcSquare == 4 && destSquare == 6 ) {
        // King already moved
        if ( king->hasMoved ) return false;
        // Rook is gone or already moved
        if ( !board.squares[7] || board.squares[7]->type != ROOK || board.squares[7]->hasMoved ) return false;
        // Squares between king and rook are occupied
        if ( board.squares[5] || board.squares[6] ) return false;
        // King passes through attacked squares
        if ( _whiteAttackBoard[4] || _whiteAttackBoard[5] || _whiteAttackBoard[6] ) return false;
        return true;
    }
    /* ------------------------- Black queen side castle ------------------------ */
    if ( srcSquare == 4 && destSquare == 2 ) {
        // King already moved
        if ( king->hasMoved ) return false;
        // Rook is gone or already moved
        if ( !board.squares[0] || board.squares[0]->type != ROOK || board.squares[0]->hasMoved ) return false;
        // Squares between king and rook are occupied
        if ( board.squares[1] || board.squares[2] || board.squares[3] ) return false;
        // King passes through attacked squares
        if ( _whiteAttackBoard[2] || _whiteAttackBoard[3] || _whiteAttackBoard[4] ) return false;
        return true;
    }
    /* ------------------------- White king side castle ------------------------- */
    if ( srcSquare == 60 && destSquare == 62 ) {
        // King already moved
        if ( king->hasMoved ) return false;
        // Rook is gone or already moved
        if ( !board.squares[63] || board.squares[63]->type != ROOK || board.squares[63]->hasMoved ) return false;
        // Squares between king and rook are occupied
        if ( board.squares[61] || board.squares[62] ) return false;
        // King passes through attacked squares
        if ( _blackAttackBoard[60] || _blackAttackBoard[61] || _blackAttackBoard[62] ) return false;
        return true;
    }
    /* ------------------------- White queen side castle ------------------------ */
    if ( srcSquare == 60 && destSquare == 58 ) {
        // King already moved
        if ( king->hasMoved ) return false;
        // Rook is gone or already moved
        if ( !board.squares[56] || board.squares[56]->type != ROOK || board.squares[56]->hasMoved ) return false;
        // Squares between king and rook are occupied
        if ( board.squares[57] || board.squares[58] || board.squares[59] ) return false;
        // King passes through attacked squares
        if ( _blackAttackBoard[58] || _blackAttackBoard[59] || _blackAttackBoard[60] ) return false;
        return true;
    }

    throw std::logic_error( "Analyze castling move was called with a move that doesn't represent castling!" );
}

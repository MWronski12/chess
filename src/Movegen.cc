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

    // Reset piece attacked and defended values
    for ( SquareIndex index = 0; index < 64; index++ ) {
        board.squares[index]->attackedValue = 0;
        board.squares[index]->defendedValue = 0;
    }

    for ( SquareIndex srcSquare = 0; srcSquare < 64; srcSquare++ ) {
        auto& piece = board.squares[srcSquare];
        if ( piece ) {
            // TODO:
            // manually resize to the count of previously generated moves to avoid dynamic resizing
            // requires adding piece.lastValidMovesCount to the Piece class
            // Clear the previous valid moves
            piece->validMoves.clear();

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

int PieceValidMoves::generateValidPawnMoves( Board& board, SquareIndex srcSquare ) {
    int movesGeneratedCount = 0;
    auto& pawn = board.squares[srcSquare];

    for ( auto& ray : _pieceMoves.getMoveList( pawn->getColor(), PAWN, srcSquare ) ) {
        for ( auto& destSquare : ray ) {
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
        for ( auto& destSquare : ray ) {
            // Castling moves have to be generated last so skip for now
            if ( abs( destSquare - srcSquare ) == 2 ) {
                continue;
            }
            // Normal move
            else if ( analyzeMove( board, srcSquare, destSquare ) && king->getColor() == board.sideToMove ) {
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
    if ( srcSquare == 60 ) {
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
    if ( abs( destSquare - srcSquare ) == 7 ) {
        pawnMoving->getColor() == WHITE ? _whiteAttackBoard[destSquare] = true : _blackAttackBoard[destSquare] = true;
    }

    /* ------------------------------- En passant ------------------------------- */
    if ( abs( destSquare - srcSquare ) == 7 && destSquare == board.getEnPassantSquare() ) {
        return true;
    }
    /* ---------------------------- Diagonal capture ---------------------------- */
    else if ( abs( destSquare - srcSquare ) == 7 ) {
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
        } else {
            return true;
        }
    }
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

/* -------------------------------------------------------------------------- */
/*                                 Piece Moves                                */
/* -------------------------------------------------------------------------- */

const std::vector<std::vector<SquareIndex>>& PieceMoves::getMoveList( PieceColor color, PieceType piece,
                                                                      SquareIndex squareIndex ) const {
    switch ( piece ) {
        case PAWN:
            return color == WHITE ? _whitePawnMoves[squareIndex] : _blackPawnMoves[squareIndex];
        case KNIGHT:
            return _knightMoves[squareIndex];
        case BISHOP:
            return _bishopMoves[squareIndex];
        case ROOK:
            return _rookMoves[squareIndex];
        case QUEEN:
            return _queenMoves[squareIndex];
        case KING:
            return _kingMoves[squareIndex];
        default:
            throw std::logic_error( "Request for moves of incorrect piece type!" );
            break;
    }
}

PieceMoves::PieceMoves() {
    generateWhitePawnMoves();
    generateBlackPawnMoves();
    generateKnightMoves();
    generateBishopMoves();
    generateRookMoves();
    generateQueenMoves();
    generateKingMoves();
}

// Helper method to convert row/col number to square index
static SquareIndex Position( SquareIndex col, SquareIndex row ) { return col + ( row * 8 ); }

void PieceMoves::generateWhitePawnMoves() {
    for ( int index = 0; index <= 64; index++ ) {
        // Pawns cannot stand on 1st and 8th ranks
        if ( index < 8 || index > 55 ) {
            continue;
        }

        int x = (int)( index % 8 );
        int y = (int)( ( index / 8 ) );

        std::vector<SquareIndex> ray;

        // Diagonal kill to the left
        if ( x > 0 && y > 0 ) {
            ray.push_back( index - 8 - 1 );
            _whitePawnMoves[index].push_back( ray );
        }
        ray.clear();

        // Forward moves
        ray.push_back( index - 8 );  // 1 forward
        if ( y == 6 ) {
            ray.push_back( index - 16 );  // 2 forward
        }
        _whitePawnMoves[index].push_back( ray );
        ray.clear();

        // Diagonal Kill to the right
        if ( x < 7 && y > 0 ) {
            ray.push_back( index - 8 + 1 );
            _whitePawnMoves[index].push_back( ray );
        }
        ray.clear();
    }
}

void PieceMoves::generateBlackPawnMoves() {
    for ( int index = 0; index <= 64; index++ ) {
        // Pawns cannot stand on 1st and 8th ranks
        if ( index < 8 || index > 55 ) {
            continue;
        }

        int x = (int)( index % 8 );
        int y = (int)( ( index / 8 ) );

        std::vector<SquareIndex> ray;

        // Diagonal kill to the left
        if ( x > 0 && y > 0 ) {
            ray.push_back( index + 8 - 1 );
            _blackPawnMoves[index].push_back( ray );
        }
        ray.clear();

        // Forward moves
        ray.push_back( index + 8 );  // 1 forward
        if ( y == 6 ) {
            ray.push_back( index + 16 );  // 2 forward
        }
        _blackPawnMoves[index].push_back( ray );
        ray.clear();

        // Diagonal Kill to the right
        if ( x < 7 && y > 0 ) {
            ray.push_back( index + 8 + 1 );
            _blackPawnMoves[index].push_back( ray );
        }
        ray.clear();
    }
}

void PieceMoves::generateKnightMoves() {
    for ( SquareIndex y = 0; y < 8; y++ ) {
        for ( SquareIndex x = 0; x < 8; x++ ) {
            SquareIndex index = ( SquareIndex )( y + ( x * 8 ) );

            SquareIndex move;
            std::vector<SquareIndex> ray;

            if ( y < 6 && x > 0 ) {
                move = Position( ( y + 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }

            if ( y > 1 && x < 7 ) {
                move = Position( ( y - 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 1 && x > 0 ) {
                move = Position( ( y - 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 6 && x < 7 ) {
                move = Position( ( y + 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 0 && x < 6 ) {
                move = Position( ( y - 1 ), ( x + 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 7 && x > 1 ) {
                move = Position( ( y + 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 0 && x > 1 ) {
                move = Position( ( y - 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 7 && x < 6 ) {
                move = Position( ( y + 1 ), ( x + 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
        }
    }
}

void PieceMoves::generateBishopMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            std::vector<SquareIndex> ray;

            // Bottom right diagonal ray
            while ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _bishopMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Bottom left diagonal ray
            while ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _bishopMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top right diagonal ray
            while ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _bishopMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top left diagonal ray
            while ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _bishopMoves[index].push_back( ray );
            ray.clear();
        }
    }
}

void PieceMoves::generateRookMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            std::vector<SquareIndex> ray;

            // Bottom side ray
            while ( row < 7 ) {
                row++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            while ( row > 0 ) {
                row--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            while ( col > 0 ) {
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Right side ray
            while ( col < 7 ) {
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
        }
    }
}

void PieceMoves::generateQueenMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            std::vector<SquareIndex> ray;

            // Bottom sie ray
            while ( row < 7 ) {
                row++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            while ( row > 0 ) {
                row--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            while ( col > 0 ) {
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Right side ray
            while ( col < 7 ) {
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Botton right diagonal ray
            while ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Bottom left diagonal ray
            while ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top right diagonal ray
            while ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top left diagonal ray
            while ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
        }
    }
}

void PieceMoves::generateKingMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            std::vector<SquareIndex> ray;

            // Bottom side ray
            if ( row < 7 ) {
                row++;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            if ( row > 0 ) {
                row--;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            if ( col > 0 ) {
                col--;
                move = Position( col, row );
                ray.push_back( move );
                // Queen side castle
                if ( index == 4 ) {
                    ray.push_back( 2 );
                } else if ( index == 60 ) {
                    ray.push_back( 58 );
                }
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Right side ray
            if ( col < 7 ) {
                col++;
                move = Position( col, row );
                ray.push_back( move );
                // King side castle
                if ( index == 4 ) {
                    ray.push_back( 6 );
                } else if ( index == 60 ) {
                    ray.push_back( 62 );
                }
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Bottom right diagonal ray
            if ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Bottom left diagonal ray
            if ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Top right diagonal ray
            if ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Top left diagonal ray
            if ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
        }
    }
}

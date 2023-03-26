#include "Movegen.h"

PieceValidMoves::PieceValidMoves( PieceMovesInterface &movesIterator ) : movesIterator( movesIterator ) {}
static SquareIndex Position( SquareIndex col, SquareIndex row ) { return col + ( row * 8 ); }

// Set correct move ray list pointer and reset indices
void PieceMovesNestedLists::start( PieceColor color, PieceType piece, SquareIndex src ) {
    switch ( piece ) {
        case PAWN:
            _currentListPointer = color == WHITE ? &whitePawnMoves[src] : &blackPawnMoves[src];
            break;

        case KNIGHT:
            _currentListPointer = &knightMoves[src];
            break;

        case BISHOP:
            _currentListPointer = &bishopMoves[src];
            break;

        case ROOK:
            _currentListPointer = &rookMoves[src];
            break;

        case QUEEN:
            _currentListPointer = &queenMoves[src];
            break;

        case KING:
            _currentListPointer = &kingMoves[src];
            break;

        default:
            throw std::logic_error( "Invalid piece requested!" );
            break;
    }
}

// Should be called before getNextMove()
bool PieceMovesNestedLists::hasNextMove() {
    if ( _rayIndex >= (int)_currentListPointer->size() ) {
        return false;
    }
    return true;
}

SquareIndex PieceMovesNestedLists::getNextMove() {
    _moveIndex++;
    if ( _moveIndex == (int)( _currentListPointer + _rayIndex )->size() ) {
        _rayIndex++;
        _moveIndex = 0;
    }
    auto rayPointer = _currentListPointer->at( _rayIndex );
    auto move = rayPointer.at( _moveIndex );
    return move;
}

void PieceMovesNestedLists::skipRay() {
    _moveIndex = 0;
    _rayIndex++;
}

// Private constructor to prevent creation of instances from outside the class.
PieceMovesNestedLists::PieceMovesNestedLists() : _rayIndex( 0 ), _moveIndex( 0 ) {
    generateWhitePawnMoves();
    generateBlackPawnMoves();
    generateKnightMoves();
    generateBishopMoves();
    generateRookMoves();
    generateQueenMoves();
    generateKingMoves();
}

void PieceMovesNestedLists::generateWhitePawnMoves() {
    for ( int index = 0; index <= 64; index++ ) {
        // Pawns cannot stand on 1st and 8th ranks
        if ( index < 8 || index > 55 ) {
            continue;
        }

        int x = (int)( index % 8 );
        int y = (int)( ( index / 8 ) );

        // Diagonal kill to the left
        if ( x > 0 && y > 0 ) {
            whitePawnMoves[index][0].push_back( index - 8 - 1 );
        }
        // One Forward
        whitePawnMoves[index][1].push_back( index - 8 );

        // Starting Position we can jump 2
        if ( y == 6 ) {
            whitePawnMoves[index][1].push_back( index - 16 );
        }
        // Diagonal Kill to the right
        if ( x < 7 && y > 0 ) {
            whitePawnMoves[index][1].push_back( index - 8 + 1 );
        }
    }
}

void PieceMovesNestedLists::generateBlackPawnMoves() {
    for ( int index = 0; index <= 64; index++ ) {
        // Pawns cannot stand on 1st and 8th ranks
        if ( index < 8 || index > 55 ) {
            continue;
        }

        int x = (int)( index % 8 );
        int y = (int)( ( index / 8 ) );

        // Diagonal kill to the left
        if ( x > 0 && y > 0 ) {
            blackPawnMoves[index][0].push_back( index + 8 - 1 );
        }
        // One Forward
        blackPawnMoves[index][1].push_back( index + 8 );

        // Starting Position we can jump 2
        if ( y == 6 ) {
            blackPawnMoves[index][1].push_back( index + 16 );
        }
        // Diagonal Kill to the right
        if ( x < 7 && y > 0 ) {
            blackPawnMoves[index][2].push_back( index + 8 + 1 );
        }
    }
}

void PieceMovesNestedLists::generateKnightMoves() {
    for ( SquareIndex y = 0; y < 8; y++ ) {
        for ( SquareIndex x = 0; x < 8; x++ ) {
            SquareIndex index = ( SquareIndex )( y + ( x * 8 ) );

            SquareIndex move;

            if ( y < 6 && x > 0 ) {
                move = Position( ( y + 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    knightMoves[index][0].push_back( move );
                }
            }
            if ( y > 1 && x < 7 ) {
                move = Position( ( y - 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    knightMoves[index][1].push_back( move );
                }
            }
            if ( y > 1 && x > 0 ) {
                move = Position( ( y - 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    knightMoves[index][2].push_back( move );
                }
            }
            if ( y < 6 && x < 7 ) {
                move = Position( ( y + 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    knightMoves[index][3].push_back( move );
                }
            }
            if ( y > 0 && x < 6 ) {
                move = Position( ( y - 1 ), ( x + 2 ) );
                if ( move < 64 ) {
                    knightMoves[index][4].push_back( move );
                }
            }
            if ( y < 7 && x > 1 ) {
                move = Position( ( y + 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    knightMoves[index][5].push_back( move );
                }
            }
            if ( y > 0 && x > 1 ) {
                move = Position( ( y - 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    knightMoves[index][6].push_back( move );
                }
            }
            if ( y < 7 && x < 6 ) {
                move = Position( ( y + 1 ), ( x + 2 ) );
                if ( move < 64 ) {
                    knightMoves[index][7].push_back( move );
                }
            }
        }
    }
}

void PieceMovesNestedLists::generateBishopMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            // Bottom right diagonal ray
            while ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                bishopMoves[index][0].push_back( move );
            }
            row = x;
            col = y;
            // Bottom left diagonal ray
            while ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                bishopMoves[index][1].push_back( move );
            }
            row = x;
            col = y;
            // Top right diagonal ray
            while ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                bishopMoves[index][2].push_back( index );
            }
            row = x;
            col = y;
            // Top left diagonal ray
            while ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                bishopMoves[index][3].push_back( index );
            }
        }
    }
}

void PieceMovesNestedLists::generateRookMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            // Bottom side ray
            while ( row < 7 ) {
                row++;
                move = Position( col, row );
                rookMoves[index][0].push_back( move );
            }
            row = x;
            col = y;
            // Top side ray
            while ( row > 0 ) {
                row--;
                move = Position( col, row );
                rookMoves[index][1].push_back( move );
            }
            row = x;
            col = y;
            // Left side ray
            while ( col > 0 ) {
                col--;
                move = Position( col, row );
                rookMoves[index][2].push_back( move );
            }
            row = x;
            col = y;
            // Right side ray
            while ( col < 7 ) {
                col++;
                move = Position( col, row );
                rookMoves[index][3].push_back( move );
            }
        }
    }
}

void PieceMovesNestedLists::generateQueenMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            // Bottom sie ray
            while ( row < 7 ) {
                row++;
                move = Position( col, row );
                queenMoves[index][0].push_back( move );
            }
            row = x;
            col = y;
            // Top side ray
            while ( row > 0 ) {
                row--;
                move = Position( col, row );
                queenMoves[index][1].push_back( move );
            }
            row = x;
            col = y;
            // Left side ray
            while ( col > 0 ) {
                col--;
                move = Position( col, row );
                queenMoves[index][2].push_back( move );
            }
            row = x;
            col = y;
            // Right side ray
            while ( col < 7 ) {
                col++;
                move = Position( col, row );
                queenMoves[index][3].push_back( move );
            }
            row = x;
            col = y;
            // Botton right diagonal ray
            while ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                queenMoves[index][4].push_back( move );
            }
            row = x;
            col = y;
            // Bottom left diagonal ray
            while ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                queenMoves[index][5].push_back( move );
            }
            row = x;
            col = y;
            // Top right diagonal ray
            while ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                queenMoves[index][6].push_back( move );
            }
            row = x;
            col = y;
            // Top left diagonal ray
            while ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                queenMoves[index][7].push_back( move );
            }
        }
    }
}

void PieceMovesNestedLists::generateKingMoves() {
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            int index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            // Bottom side ray
            if ( row < 7 ) {
                row++;
                move = Position( col, row );
                kingMoves[index][0].push_back( move );
            }
            row = x;
            col = y;
            // Top side ray
            if ( row > 0 ) {
                row--;
                move = Position( col, row );
                kingMoves[index][1].push_back( move );
            }
            row = x;
            col = y;
            // Left side ray
            if ( col > 0 ) {
                col--;
                move = Position( col, row );
                kingMoves[index][2].push_back( move );
                // Queen side castle
                if ( index == 4 ) {
                    kingMoves[index][2].push_back( 2 );
                } else if ( index == 60 ) {
                    kingMoves[index][2].push_back( 58 );
                }
            }
            row = x;
            col = y;
            // Right side ray
            if ( col < 7 ) {
                col++;
                move = Position( col, row );
                kingMoves[index][3].push_back( move );
                // King side castle
                if ( index == 4 ) {
                    kingMoves[index][3].push_back( 6 );
                } else if ( index == 60 ) {
                    kingMoves[index][3].push_back( 62 );
                }
            }
            row = x;
            col = y;
            // Bottom right diagonal ray
            if ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = Position( col, row );
                kingMoves[index][4].push_back( move );
            }
            row = x;
            col = y;
            // Bottom left diagonal ray
            if ( row < 7 && col > 0 ) {
                row++;
                col--;
                move = Position( col, row );
                kingMoves[index][5].push_back( move );
            }
            row = x;
            col = y;
            // Top right diagonal ray
            if ( row > 0 && col < 7 ) {
                row--;
                col++;
                move = Position( col, row );
                kingMoves[index][6].push_back( move );
            }
            row = x;
            col = y;
            // Top left diagonal ray
            if ( row > 0 && col > 0 ) {
                row--;
                col--;
                move = Position( col, row );
                kingMoves[index][7].push_back( move );
            }
        }
    }
}
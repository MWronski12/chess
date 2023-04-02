#include "PieceMoves.h"

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
static constexpr SquareIndex position( SquareIndex col, SquareIndex row ) { return col + ( row * 8 ); }

void PieceMoves::generateWhitePawnMoves() {
    for ( SquareIndex index = 0; index <= 64; index++ ) {
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
    for ( SquareIndex index = 0; index <= 64; index++ ) {
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
        if ( y == 1 ) {
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
    for ( int y = 0; y < 8; y++ ) {
        for ( int x = 0; x < 8; x++ ) {
            SquareIndex index = ( SquareIndex )( y + ( x * 8 ) );

            SquareIndex move;
            std::vector<SquareIndex> ray;

            if ( y < 6 && x > 0 ) {
                move = position( ( y + 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }

            if ( y > 1 && x < 7 ) {
                move = position( ( y - 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 1 && x > 0 ) {
                move = position( ( y - 2 ), ( x - 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 6 && x < 7 ) {
                move = position( ( y + 2 ), ( x + 1 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 0 && x < 6 ) {
                move = position( ( y - 1 ), ( x + 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 7 && x > 1 ) {
                move = position( ( y + 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y > 0 && x > 1 ) {
                move = position( ( y - 1 ), ( x - 2 ) );
                if ( move < 64 ) {
                    ray.push_back( move );
                    _knightMoves[index].push_back( ray );
                    ray.clear();
                }
            }
            if ( y < 7 && x < 6 ) {
                move = position( ( y + 1 ), ( x + 2 ) );
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
            SquareIndex index = (int)( y + ( x * 8 ) );
            int move;
            int row = x;
            int col = y;
            std::vector<SquareIndex> ray;

            // Bottom right diagonal ray
            while ( row < 7 && col < 7 ) {
                row++;
                col++;
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            while ( row > 0 ) {
                row--;
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            while ( col > 0 ) {
                col--;
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _rookMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Right side ray
            while ( col < 7 ) {
                col++;
                move = position( col, row );
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
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            while ( row > 0 ) {
                row--;
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            while ( col > 0 ) {
                col--;
                move = position( col, row );
                ray.push_back( move );
            }
            if ( ray.size() > 0 ) _queenMoves[index].push_back( ray );
            ray.clear();
            row = x;
            col = y;

            // Right side ray
            while ( col < 7 ) {
                col++;
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Top side ray
            if ( row > 0 ) {
                row--;
                move = position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
            row = x;
            col = y;

            // Left side ray
            if ( col > 0 ) {
                col--;
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
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
                move = position( col, row );
                ray.push_back( move );
                _kingMoves[index].push_back( ray );
            }
            ray.clear();
        }
    }
}

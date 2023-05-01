/*
 * Brief:
 * Board class representing position in the game of chess.
 * Contains all the information necessary to continue a valid game of chess.
 * Provides makeMove method.
 *
 * Author: Mikołaj Wroński
 * Date: 24.03.2023
 */

#include "Board.h"

#include <iostream>
#include <stdexcept>

#include "Search.h"

/*
                           castling   move     in      in
                              right update     binary  decimal
 king & rooks didn't move:     1111 & 1111  =  1111    15
        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13

         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7
*/

// castling rights update constants
const int castling_rights[64] = { 7,  15, 15, 15, 3,  15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14 };

/* ------------------------------ Constructors ------------------------------ */

// Creates a board with a starting position
Board::Board()
    : whiteIsChecked( false ),
      blackIsChecked( false ),
      //   whiteHasCastled( false ),
      //   blackHasCastled( false ),
      castlingRights( WHITE_KING_SIDE | WHITE_QUEEN_SIDE | BLACK_KING_SIDE | BLACK_QUEEN_SIDE ),
      whiteIsCheckMated( false ),
      blackIsCheckMated( false ),
      staleMate( false ),
      score( 0 ),
      sideToMove( WHITE ),
      lastMove( MoveContent() ),
      enPassantSquare( NULL_SQUARE ),
      fiftyMoveCounter_( 0 ),
      threefoldRepetitionCounter_( 0 ) {
    for ( SquareIndex i = 0; i < 64; i++ ) {
        if ( STARTING_POSITION[i] == EMPTY ) {
            continue;
        }

        PieceColor color;

        if ( i < 16 ) {  // Black side squares are low indieces
            color = BLACK;
        } else if ( i > 47 ) {  // White side squares are high indieces
            color = WHITE;
        }

        squares[i] = std::make_optional<Piece>( color, STARTING_POSITION[i], false );
    }
}

Board::Board( std::string fen )
    : whiteIsChecked( false ),
      blackIsChecked( false ),
      castlingRights( 0 ),
      whiteIsCheckMated( false ),
      blackIsCheckMated( false ),
      staleMate( false ),
      score( 0 ),
      threefoldRepetitionCounter_( 0 ) {
    /* ------------------------ Board squares description ----------------------- */
    int numOfSlashes = 0;         // Should be exactly 7 slashes
    SquareIndex squareIndex = 0;  // Should be exactly 64 squares
    int lastSlashIndex = -1;      // Cant be two consecutive slashes
    auto it = fen.cbegin();
    while ( it != fen.cend() && *it != ' ' ) {
        // Is piece - one of: "pnbrqkPNBRQK"
        if ( isalpha( *it ) ) {
            std::string validPieces = "pnbrqkPNBRQK";
            if ( std::find( validPieces.cbegin(), validPieces.cend(), *it ) == validPieces.cend() ) {
                throw std::invalid_argument( "Invalid FEN notation - unknown piece char" );
            }
            squares[squareIndex++] = Piece( *it );
        }
        // Is n empty squares - one of: "12345678"
        else if ( isdigit( *it ) ) {
            int numOfEmptySquares = int( *it ) - 48;
            if ( numOfEmptySquares < 1 || numOfEmptySquares > 8 ) {
                throw std::invalid_argument( "Invalid FEN notation - invalid number of empty squares" );
            }
            for ( int i = 0; i < numOfEmptySquares; i++ ) {
                squares[squareIndex++] = std::nullopt;
            }
        }
        // Is separator '/'
        else if ( *it == '/' ) {
            if ( it - fen.cbegin() - lastSlashIndex == 1 || ++numOfSlashes > 7 ) {
                throw std::invalid_argument( "Invalid FEN notation - invalid number of slashes" );
            }
            lastSlashIndex = it - fen.cbegin();
        }
        // Anyhing else is invalid
        else {
            throw std::invalid_argument( "Invalid FEN notation - invalid char encountered" );
        }
        ++it;
    }

    if ( squareIndex != 64 || numOfSlashes != 7 ) {
        throw std::invalid_argument( "Invalid FEN notation - invalid number of squares described or slashes" );
    }

    /* ------------------------------ side to move ------------------------------ */
    if ( ++it == fen.cend() ) throw std::invalid_argument( "Invalid FEN notation - side to move required" );

    if ( *it == 'w' ) {
        sideToMove = WHITE;
    } else if ( *it == 'b' ) {
        sideToMove = BLACK;
    } else {
        throw std::invalid_argument( "Invalid FEN notation - invalid side to move" );
    }

    if ( ++it == fen.cend() || *it != ' ' ) throw std::invalid_argument( "Invalid FEN notation - castling rights required" );

    /* ----------------------------- Castling rights ---------------------------- */
    while ( ++it != fen.cend() && *it != ' ' ) {
        if ( *it == '-' ) {
            // blackHasCastled = true;
            // whiteHasCastled = true;
            castlingRights = 0;
            continue;
        } else if ( *it == 'K' ) {
            if ( !squares[63] || squares[63]->type != ROOK || squares[63]->color != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white king side castling requires a rook on H1" );
            }
            castlingRights |= WHITE_KING_SIDE;
        } else if ( *it == 'Q' ) {
            if ( !squares[56] || squares[56]->type != ROOK || squares[56]->color != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white queen side castling requires a rook on A1" );
            }
            castlingRights |= WHITE_QUEEN_SIDE;
        } else if ( *it == 'k' ) {
            if ( !squares[7] || squares[7]->type != ROOK || squares[7]->color != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on H8" );
            }
            castlingRights |= BLACK_KING_SIDE;
        } else if ( *it == 'q' ) {
            if ( !squares[0] || squares[0]->type != ROOK || squares[0]->color != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on A8" );
            }
            castlingRights |= BLACK_QUEEN_SIDE;
        } else {
            throw std::invalid_argument( "Invalid FEN notation - invalid castling rights" );
        }
    }

    /* ------------------------------- En passant ------------------------------- */
    if ( ++it == fen.cend() ) throw std::invalid_argument( "Invalid FEN notation - en passant required" );

    if ( *it == '-' ) {
        ++it;
    } else {
        char file = tolower( *it );
        if ( ++it == fen.cend() ) throw std::invalid_argument( "Invalid FEN notation - en passant square description" );
        char rank = *it;
        if ( file < 'a' || file > 'h' || rank < '1' || rank > '8' ) {
            throw std::invalid_argument( "Invalid FEN notation - en passant square description" );
        }
        enPassantSquare = 8 * ( 8 - ( int( rank ) - 48 ) ) + ( int( file ) - 97 );
    }

    if ( it == fen.cend() || *it != ' ' ) throw std::invalid_argument( "Invalid FEN notation - fifty move count required" );

    /* --------------------------- fifty move counter --------------------------- */
    std::string fiftyMoveCount;
    while ( ++it != fen.cend() && *it != ' ' ) {
        fiftyMoveCount += *it;
    }
    fiftyMoveCounter_ = std::stoi( fiftyMoveCount );
    if ( it == fen.cend() ) throw std::invalid_argument( "Invalid FEN notation - turn count required" );

    /* ------------------------------- turn count ------------------------------- */
    std::string turnCount;
    while ( ++it != fen.cend() ) {
        turnCount += *it;
    }
    initZobrist();
    hash_key = getHashKey();
}

Board Board::fastCopy() const {
    Board copy;
    copy.sideToMove = this->sideToMove;
    // copy.whiteHasCastled = this->whiteHasCastled;
    // copy.blackHasCastled = this->blackHasCastled;
    copy.castlingRights = this->castlingRights;
    copy.fiftyMoveCounter_ = this->fiftyMoveCounter_;
    copy.lastMove = this->lastMove;
    copy.threefoldRepetitionCounter_ = this->threefoldRepetitionCounter_;
    for ( int i = 0; i < 64; i++ ) {
        if ( this->squares[i] != std::nullopt ) {
            auto piece = Piece( this->squares[i]->color, this->squares[i]->type, this->squares[i]->hasMoved );
            copy.squares[i] = std::make_optional<Piece>( piece );
        }
    }
    return copy;
}

/* --------------------------------- Methods -------------------------------- */

void Board::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    validateMove( src, dest, promotion );

    PieceType pieceMoving = squares[src] ? squares[src]->type : EMPTY;
    PieceType pieceTaken = squares[dest] ? squares[dest]->type : EMPTY;
    PieceColor colorMoving = squares[src]->color;

    // Record move details
    lastMove.src = src;
    lastMove.dest = dest;
    lastMove.pieceMoving = pieceMoving;
    lastMove.pieceTaken = pieceTaken;
    lastMove.promotion = promotion;
    lastMove.isEnPassantCapture = false;

    hash_key ^= piece_keys[colorMoving][pieceMoving][src];   // remove piece from source square in hash key
    hash_key ^= piece_keys[colorMoving][pieceMoving][dest];  // set piece to the target square in hash key
    if ( pieceTaken != EMPTY ) hash_key ^= piece_keys[1 - colorMoving][pieceTaken][dest];  // remove the piece from hash key

    // Detect special move scenario and handle extra behaviour
    // Pawn move by 2
    if ( pieceMoving == PAWN && abs( src - dest ) == 16 ) {
        recordEnPassant( src, dest );
    }
    // En passant capture
    else if ( enPassantIsAvailable() && pieceMoving == PAWN && dest == enPassantSquare ) {
        handleEnPassant();
    }
    // Castling
    else if ( pieceMoving == KING && abs( src - dest ) == 2 ) {
        handleCastling( src, dest );
    }
    // Pawn promotion
    else if ( pieceMoving == PAWN && ( dest < 8 || dest > 55 ) ) {
        handlePromotion( src, dest, promotion );
    }

    // Handle behaviour common to every move
    squares[dest] = std::move( squares[src] );
    squares[src] = std::nullopt;
    squares[dest]->hasMoved = true;

    // Update side to move
    sideToMove = sideToMove == WHITE ? BLACK : WHITE;

    // Clear enPassantSquare
    if ( pieceMoving != PAWN || abs( src - dest ) != 16 ) {
        hash_key ^= enpassant_keys[enPassantSquare];
        enPassantSquare = NULL_SQUARE;
    }

    // Update 50 repetition counter
    if ( pieceMoving != PAWN && pieceTaken == EMPTY ) {
        fiftyMoveCounter_++;
        if ( fiftyMoveCounter_ == 50 ) {
            staleMate = true;
            return;
        }
    } else {
        fiftyMoveCounter_ = 0;
    }

    // hash castling
    hash_key ^= castle_keys[castlingRights];

    // update castling rights
    castlingRights &= castling_rights[src];
    castlingRights &= castling_rights[dest];

    // hash castling
    hash_key ^= castle_keys[castlingRights];

    hash_key ^= side_key;
    // TODO: Update threefoldRepetitionCounter
}

// TODO: separate conversion of the move representation
// D2D4 notation (D2D4Q for promotion)
void Board::makeMove( std::string move ) {
    // Calculate and validate SquareIndex
    int src = 8 * ( 8 - ( int( move[1] ) - 48 ) ) + ( int( toupper( move[0] ) ) - 65 );
    int dest = 8 * ( 8 - ( int( move[3] ) - 48 ) ) + ( int( toupper( move[2] ) ) - 65 );
    if ( src < 0 || src > 63 || dest < 0 || dest > 63 ) {
        throw std::invalid_argument( "Invalid square notation!" );
    }

    PieceType promotion;

    // Normal move like d2d4
    if ( move.size() == 4 ) {
        makeMove( src, dest, EMPTY );
    }
    // Promotion move like d7d8q
    else if ( move.size() == 5 ) {
        switch ( toupper( move[4] ) ) {
            case 'Q':
                promotion = QUEEN;
                break;

            case 'R':
                promotion = ROOK;
                break;

            case 'B':
                promotion = BISHOP;
                break;

            case 'N':
                promotion = KNIGHT;
                break;

            default:
                throw std::invalid_argument( "Invalid promotion type!" );
                break;
        }
        makeMove( src, dest, promotion );
    }
    // Invalid move
    else {
        throw std::invalid_argument( "Invalid move notation!" );
    }
}

/* ------------------------- makeMove helper methods ------------------------ */

// Returns true if enPassant is available
bool Board::enPassantIsAvailable() const {
    if ( enPassantSquare == NULL_SQUARE ) {
        return false;
    }
    return true;
}

// Validates that:
// - game is not over yet
// - src and dest are valid square indices
// - src square is occupied by the allied piece
// - dest square is not occupied by allied piece
// - promotion is not EMPTY only if this is a promoting move
void Board::validateMove( SquareIndex src, SquareIndex dest, PieceType promotion ) const {
    // Check if game is over
    if ( staleMate || blackIsCheckMated || whiteIsCheckMated ) {
        throw std::logic_error( "The game is already over!" );
    }
    // Validate square indices
    if ( src > 63 || dest > 63 ) {
        throw std::invalid_argument( "Invalid square indices!" );
    }
    // Validate source square piece
    if ( !squares[src] ) {
        throw std::logic_error( "Source square must be occupied by a piece!" );
    } else if ( sideToMove != squares[src]->color ) {
        throw std::logic_error( "Source square must be occupied by a piece owned by the player whose turn it is!" );
    }
    // Validate dest square piece
    if ( squares[dest] ) {
        if ( sideToMove == squares[dest]->color ) {
            throw std::logic_error( "Capturing allied piece is not allowed!" );
        }
    }
    // Validate promotion logic
    if ( squares[src]->type == PAWN && ( dest < 8 || dest > 55 ) ) {
        if ( promotion == EMPTY ) {
            throw std::logic_error( "Promotion PieceType must be specified for promoting move!" );
        }
    } else if ( promotion != EMPTY ) {
        throw std::logic_error( "Promotion PieceType must be EMPTY for non-promoting move!" );
    }
}

// Sets en passant square according to destination square
void Board::recordEnPassant( SquareIndex src, SquareIndex dest ) {
    if ( squares[src]->color == WHITE ) {
        enPassantSquare = dest + 8;
    } else if ( squares[src]->color == BLACK ) {
        enPassantSquare = dest - 8;
    }
    hash_key ^= enpassant_keys[enPassantSquare];
}

// Clears enPassant square and records additional lastMove details
void Board::handleEnPassant() {
    if ( sideToMove == WHITE ) {
        squares[enPassantSquare + 8] = std::nullopt;
        hash_key ^= piece_keys[WHITE][PAWN][enPassantSquare + 8];  // remove pawn from hash key
    } else if ( sideToMove == BLACK ) {
        squares[enPassantSquare - 8] = std::nullopt;
        hash_key ^= piece_keys[BLACK][PAWN][enPassantSquare - 8];  // remove pawn from hash key
    }
    lastMove.isEnPassantCapture = true;
    lastMove.pieceTaken = PAWN;
}

// Moves the rook according to the castling move being made
void Board::handleCastling( SquareIndex src, SquareIndex dest ) {
    // white castle
    if ( src == 60 ) {
        // king side
        if ( dest == 62 ) {
            squares[61] = std::move( squares[63] );
            squares[61]->hasMoved = true;
            squares[63] = std::nullopt;

            // hash rook
            hash_key ^= piece_keys[WHITE][ROOK][63];  // remove rook from h1 from hash key
            hash_key ^= piece_keys[WHITE][ROOK][61];  // put rook on f1 into a hash key
        }
        // queen side
        if ( dest == 58 ) {
            squares[59] = std::move( squares[56] );
            squares[59]->hasMoved = true;
            squares[56] = std::nullopt;

            // hash rook
            hash_key ^= piece_keys[WHITE][ROOK][56];  // remove rook from h1 from hash key
            hash_key ^= piece_keys[WHITE][ROOK][59];  // put rook on f1 into a hash key
        }
    }
    // black castle
    else if ( src == 4 ) {
        // king side
        if ( dest == 6 ) {
            squares[5] = std::move( squares[7] );
            squares[5]->hasMoved = true;
            squares[7] = std::nullopt;

            // hash rook
            hash_key ^= piece_keys[BLACK][ROOK][7];  // remove rook from h1 from hash key
            hash_key ^= piece_keys[BLACK][ROOK][5];  // put rook on f1 into a hash key
        }
        // queen side
        else if ( dest == 2 ) {
            squares[3] = std::move( squares[0] );
            squares[3]->hasMoved = true;
            squares[0] = std::nullopt;

            // hash rook
            hash_key ^= piece_keys[BLACK][ROOK][0];  // remove rook from h1 from hash key
            hash_key ^= piece_keys[BLACK][ROOK][3];  // put rook on f1 into a hash key
        }
    }
}

// Changes type of promoted piece, while its still on the src square
void Board::handlePromotion( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    hash_key ^= piece_keys[squares[src]->color][squares[src]->type][dest];  // remove pawn

    squares[src]->type = promotion;
    squares[src]->value = Piece::calculatePieceValue( promotion );
    squares[src]->actionValue = Piece::calculatePieceActionValue( promotion );

    hash_key ^= piece_keys[squares[src]->color][promotion][dest];  // add promoted piece
}
/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                       Zobrist                                                      */
/* ------------------------------------------------------------------------------------------------------------------ */
void Board::initZobrist() {
    std::mt19937_64 gen( std::random_device{}() );
    std::uniform_int_distribution<uint64_t> dis( 0, UINT64_MAX );
    for ( int y = 0; y < 2; y++ ) {
        for ( int i = 0; i < 6; i++ ) {
            for ( int j = 0; j < 64; j++ ) {
                piece_keys[y][i][j] = dis( gen );
            }
        }
    }
    side_key = dis( gen );
    for ( int i = 0; i < 16; i++ ) {
        castle_keys[i] = dis( gen );
    }
    for ( int i = 0; i < 64; i++ ) {
        enpassant_keys[i] = dis( gen );
    }
}

uint64_t Board::getHashKey() {
    uint64_t final_key = 0;
    for ( int i = 0; i < 64; i++ ) {
        const auto& piece = squares[i];
        if ( piece.has_value() ) {
            PieceColor color = piece->color;
            final_key ^= piece_keys[color][piece->type][i];
        }
    }
    if ( sideToMove == BLACK ) {
        final_key ^= side_key;
    }
    final_key ^= castle_keys[castlingRights];

    if ( enPassantSquare != NULL_SQUARE ) final_key ^= enpassant_keys[enPassantSquare];
    return final_key;  // initialize at start of the game ( in board initialization for example), update after each move
    // HASH_KEY SHOULD BE IN BOARD CLASS!!, but then it is not possible to pass board as const bcs we will update it
}

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

/* ------------------------------ Constructors ------------------------------ */

// Creates a board with a starting position
Board::Board()
    : whiteIsChecked( false ),
      blackIsChecked( false ),
      whiteHasCastled( false ),
      blackHasCastled( false ),
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
            blackHasCastled = true;
            whiteHasCastled = true;
            continue;
        } else if ( *it == 'K' ) {
            if ( !squares[63] || squares[63]->type != ROOK || squares[63]->color != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white king side castling requires a rook on H1" );
            }
            whiteHasCastled = false;
        } else if ( *it == 'Q' ) {
            if ( !squares[56] || squares[56]->type != ROOK || squares[56]->color != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white queen side castling requires a rook on A1" );
            }
            whiteHasCastled = false;
        } else if ( *it == 'k' ) {
            if ( !squares[7] || squares[7]->type != ROOK || squares[7]->color != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on H8" );
            }
            blackHasCastled = false;
        } else if ( *it == 'q' ) {
            if ( !squares[0] || squares[0]->type != ROOK || squares[0]->color != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on A8" );
            }
            blackHasCastled = false;
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

    hash_key = getHashKey();
}

Board Board::fastCopy() const {
    Board copy;
    copy.sideToMove = this->sideToMove;
    copy.whiteHasCastled = this->whiteHasCastled;
    copy.blackHasCastled = this->blackHasCastled;
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

    // Record move details
    lastMove.src = src;
    lastMove.dest = dest;
    lastMove.pieceMoving = pieceMoving;
    lastMove.pieceTaken = pieceTaken;
    lastMove.promotion = promotion;
    lastMove.isEnPassantCapture = false;

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
        handlePromotion( src, promotion );
    }

    // Handle behaviour common to every move
    squares[dest] = std::move( squares[src] );
    squares[src] = std::nullopt;
    squares[dest]->hasMoved = true;

    // Update side to move
    sideToMove = sideToMove == WHITE ? BLACK : WHITE;

    // Clear enPassantSquare
    if ( pieceMoving != PAWN || abs( src - dest ) != 16 ) {
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
}

// Clears enPassant square and records additional lastMove details
void Board::handleEnPassant() {
    if ( sideToMove == WHITE ) {
        squares[enPassantSquare + 8] = std::nullopt;
    } else if ( sideToMove == BLACK ) {
        squares[enPassantSquare - 8] = std::nullopt;
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
        }
        // queen side
        if ( dest == 58 ) {
            squares[59] = std::move( squares[56] );
            squares[59]->hasMoved = true;
            squares[56] = std::nullopt;
        }
        whiteHasCastled = true;
    }
    // black castle
    else if ( src == 4 ) {
        // king side
        if ( dest == 6 ) {
            squares[5] = std::move( squares[7] );
            squares[5]->hasMoved = true;
            squares[7] = std::nullopt;
        }
        // queen side
        else if ( dest == 2 ) {
            squares[3] = std::move( squares[0] );
            squares[3]->hasMoved = true;
            squares[0] = std::nullopt;
        }
        blackHasCastled = true;
    }
}

// Changes type of promoted piece, while its still on the src square
void Board::handlePromotion( SquareIndex src, PieceType promotion ) {
    squares[src]->type = promotion;
    squares[src]->value = Piece::calculatePieceValue( promotion );
    squares[src]->actionValue = Piece::calculatePieceActionValue( promotion );
}
/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                       Zobrist                                                      */
/* ------------------------------------------------------------------------------------------------------------------ */
void Board::initZobrist() {
    std::mt19937_64 gen( std::random_device{}() );
    std::uniform_int_distribution<uint64_t> dis( 0, UINT64_MAX );
    for ( int i = 0; i < 12; i++ ) {
        for ( int j = 0; j < 64; j++ ) {
            piece_keys[i][j] = dis( gen );
        }
    }
    side_key = dis( gen );
    for ( int i = 0; i < 4; i++ ) {
        castling_keys[i] = dis( gen );
    }
    // tez enpassant
}

uint64_t Board::getHashKey() {
    uint64_t hash_key = 0;
    for ( int i = 0; i < 64; i++ ) {
        const auto& piece = squares[i];
        if ( piece.has_value() ) {
            PieceColor color = piece->color;
            hash_key ^= piece_keys[color * 6 + piece->type][i];
        }
    }
    if ( sideToMove == BLACK ) {
        hash_key ^= side_key;
    }
    // if ( board.get_castling_rights() & Chess::WHITE_KINGSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[0];
    // }
    // if ( board.get_castling_rights() & Chess::WHITE_QUEENSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[1];
    // }
    // if ( board.get_castling_rights() & Chess::BLACK_KINGSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[2];
    // }
    // if ( board.get_castling_rights() & Chess::BLACK_QUEENSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[3];
    // }

    // TODO: enpassant
    return hash_key;  // initialize at start of the game ( in board initialization for example), update after each move
    // HASH_KEY SHOULD BE IN BOARD CLASS!!, but then it is not possible to pass board as const bcs we will update it
}

// function should be called after move is made
// do we have to restore key after alphaBeta?? No bcs we copy the board, to be confirmed
uint64_t Board::updateHashKey( uint64_t hash_key, const MoveContent& move ) {
    SquareIndex src = move.src;
    SquareIndex dest = move.dest;
    const auto& piece = squares[src];
    if ( !piece.has_value() ) {
        return hash_key;
    }
    PieceColor color = piece->color;
    hash_key ^= piece_keys[color * 6 + piece->type][src];
    if ( move.promotion != EMPTY ) {
        PieceType promoted_piece = move.promotion;
        hash_key ^= piece_keys[color * 6 + promoted_piece][dest];
    } else {
        hash_key ^= piece_keys[color * 6 + piece->type][dest];
    }
    if ( move.pieceTaken != EMPTY ) {
        SquareIndex capture_square = dest;
        PieceType captured_piece = move.pieceTaken;
        PieceColor capture_color = BLACK == color ? WHITE : BLACK;
        hash_key ^= piece_keys[capture_color * 6 + captured_piece][capture_square];
    }
    if ( sideToMove == WHITE ) {
        hash_key ^= side_key;
    }
    // if ( chess.get_castling_rights() & Chess::WHITE_KINGSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[0];
    // }
    // if ( chess.get_castling_rights() & Chess::WHITE_QUEENSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[1];
    // }
    // if ( chess.get_castling_rights() & Chess::BLACK_KINGSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[2];
    // }
    // if ( chess.get_castling_rights() & Chess::BLACK_QUEENSIDE_CASTLE ) {
    //     hash_key ^= castling_keys[3];
    // }
    return hash_key;
}
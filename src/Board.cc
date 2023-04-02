/*
 * Brief:
 * Board class representing position in the game of chess.
 * Contains all the information necessary to continue a valid game of chess.
 * Provides makeMove method.
 *
 * Author: Mikołaj Wroński
 * Date: 24.03.2023
 */

#include <iostream>
#include <stdexcept>

#include "Board.h"

/* -------------------------------------------------------------------------- */
/*                                Constructors                                */
/* -------------------------------------------------------------------------- */

// Creates a board with a starting position
Board::Board()
    : sideToMove( WHITE ),
      whiteIsChecked( false ),
      blackIsChecked( false ),
      whiteIsCheckMated( false ),
      blackIsCheckMated( false ),
      staleMate( false ),
      score( 0 ),
      _whiteHasCastled( false ),
      _blackHasCastled( false ),
      _enPassantSquare( NULL_SQUARE ),
      _fiftyMoveCounter( 0 ),
      _threefoldRepetitionCounter( 0 ) {
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
      _threefoldRepetitionCounter( 0 ) {
    /* ------------------------ Board squares description ----------------------- */
    int numOfSlashes = 0;         // Should be exactly 7 slashes
    SquareIndex squareIndex = 0;  // Should be exactly 64 squares
    int lastSlashIndex = -1;      // Cant be two consecutive slashes
    auto it = fen.cbegin();
    while ( it != fen.cend() && *it != ' ' ) {
        // Is piece "pnbrqkPNBRQK"
        if ( isalpha( *it ) ) {
            std::string validPieces = "pnbrqkPNBRQK";
            if ( std::find( validPieces.cbegin(), validPieces.cend(), *it ) == validPieces.cend() ) {
                throw std::invalid_argument( "Invalid FEN notation - unknown piece char" );
            }
            squares[squareIndex++] = Piece( *it );
        }
        // Is empty squares "12345678"
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

    if ( ++it == fen.cend() || *it != ' ' )
        throw std::invalid_argument( "Invalid FEN notation - castling rights required" );

    /* ----------------------------- Castling rights ---------------------------- */
    while ( ++it != fen.cend() && *it != ' ' ) {
        if ( *it == '-' ) {
            _blackHasCastled = true;
            _whiteHasCastled = true;
            continue;
        } else if ( *it == 'K' ) {
            if ( !squares[63] || squares[63]->type != ROOK || squares[63]->getColor() != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white king side castling requires a rook on H1" );
            }
            _whiteHasCastled = false;
        } else if ( *it == 'Q' ) {
            if ( !squares[56] || squares[56]->type != ROOK || squares[56]->getColor() != WHITE ) {
                throw std::invalid_argument( "Invalid FEN notation - white queen side castling requires a rook on A1" );
            }
            _whiteHasCastled = false;
        } else if ( *it == 'k' ) {
            if ( !squares[7] || squares[7]->type != ROOK || squares[7]->getColor() != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on H8" );
            }
            _blackHasCastled = false;
        } else if ( *it == 'q' ) {
            if ( !squares[0] || squares[0]->type != ROOK || squares[0]->getColor() != BLACK ) {
                throw std::invalid_argument( "Invalid FEN notation - black king side castling requires a rook on A8" );
            }
            _blackHasCastled = false;
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
        _enPassantSquare = 8 * ( 8 - ( int( rank ) - 48 ) ) + ( int( file ) - 97 );
    }

    if ( it == fen.cend() || *it != ' ' )
        throw std::invalid_argument( "Invalid FEN notation - fifty move count required" );

    /* --------------------------- fifty move counter --------------------------- */
    std::string fiftyMoveCount;
    while ( ++it != fen.cend() && *it != ' ' ) {
        fiftyMoveCount += *it;
    }
    _fiftyMoveCounter = std::stoi( fiftyMoveCount );
    if ( it == fen.cend() ) throw std::invalid_argument( "Invalid FEN notation - turn count required" );

    /* ------------------------------- turn count ------------------------------- */
    std::string turnCount;
    while ( ++it != fen.cend() ) {
        turnCount += *it;
    }
}

Board Board::fastCopy() {
    Board newBoard;
    newBoard.sideToMove = this->sideToMove;
    newBoard.whiteIsChecked = this->whiteIsChecked;
    newBoard.blackIsChecked = this->blackIsChecked;
    newBoard._blackHasCastled = this->_blackHasCastled;
    newBoard._whiteHasCastled = this->_whiteHasCastled;
    newBoard._fiftyMoveCounter = this->_fiftyMoveCounter;
    newBoard._threefoldRepetitionCounter = this->_fiftyMoveCounter;
    // Copy the pieces, including only their color, type and hasMoved fields
    for ( SquareIndex i = 0; i < 64; i++ ) {
        if ( this->squares[i] ) {
            auto &piece = this->squares[i];
            newBoard.squares[i] = std::make_optional<Piece>( piece->getColor(), piece->type, piece->hasMoved );
        }
    }
    return newBoard;
}

/* -------------------------------------------------------------------------- */
/*                                   getters                                  */
/* -------------------------------------------------------------------------- */

bool Board::whiteHasCastled() const { return _whiteHasCastled; }
bool Board::blackHasCastled() const { return _blackHasCastled; }
SquareIndex Board::getEnPassantSquare() const { return _enPassantSquare; }

/* -------------------------------------------------------------------------- */
/*                               makeMove methods                             */
/* -------------------------------------------------------------------------- */

void Board::makeMove( SquareIndex src, SquareIndex dest, PieceType promotion ) {
    validateMove( src, dest, promotion );

    PieceType pieceMoving = squares[src] ? squares[src]->type : EMPTY;
    PieceType pieceTaken = squares[dest] ? squares[dest]->type : EMPTY;

    // TODO: three fold repetition rule
    if ( _fiftyMoveCounter == 50 || _threefoldRepetitionCounter == 3 ) {
        staleMate = true;
        return;
    }

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
    else if ( enPassantIsAvailable() && pieceMoving == PAWN && dest == getEnPassantSquare() ) {
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

    // Update other board information
    sideToMove = sideToMove == WHITE ? BLACK : WHITE;
    if ( pieceMoving != PAWN || abs( src - dest ) != 16 ) {
        _enPassantSquare = NULL_SQUARE;
    }
    if ( pieceMoving != PAWN && pieceTaken == EMPTY ) {
        _fiftyMoveCounter++;
    } else {
        _fiftyMoveCounter = 0;
    }
}

/* -------------------------------------------------------------------------- */
/*                           makeMove helper methods                          */
/* -------------------------------------------------------------------------- */
// Returns true if enPassant is available
bool Board::enPassantIsAvailable() const {
    if ( _enPassantSquare == NULL_SQUARE ) {
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
    } else if ( sideToMove != squares[src]->getColor() ) {
        throw std::logic_error( "Source square must be occupied by a piece owned by the player whose turn it is!" );
    }
    // Validate dest square piece
    if ( squares[dest] ) {
        if ( sideToMove == squares[dest]->getColor() ) {
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
    if ( squares[src]->getColor() == WHITE ) {
        _enPassantSquare = dest + 8;
    } else if ( squares[src]->getColor() == BLACK ) {
        _enPassantSquare = dest - 8;
    }
}

// Clears enPassant square and records additional lastMove details
void Board::handleEnPassant() {
    if ( sideToMove == WHITE ) {
        squares[getEnPassantSquare() + 8] = std::nullopt;
    } else if ( sideToMove == BLACK ) {
        squares[getEnPassantSquare() - 8] = std::nullopt;
    }
    lastMove.isEnPassantCapture = true;
    lastMove.pieceTaken = PAWN;
}

// Moves the rook
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
        _whiteHasCastled = true;
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
        _blackHasCastled = true;
    }
}

// Changes type of promoted piece, while its still on the src square
void Board::handlePromotion( SquareIndex src, PieceType promotion ) {
    squares[src]->type = promotion;
    squares[src]->setPieceValue();
    squares[src]->setPieceActionValue();
}

#include <cstdint>
#include <random>

#include "chess.h"

uint64_t piece_keys[12][64];
uint64_t side_key;
uint64_t castling_keys[4];

void init_zobrist() {
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
}

uint64_t get_hash_key( const Chess& chess ) {
    uint64_t hash_key = 0;
    for ( int i = 0; i < 64; i++ ) {
        char piece = chess.get_piece( i );
        if ( piece != Chess::EMPTY ) {
            int color = chess.get_color( i );
            hash_key ^= piece_keys[color * 6 + piece][i];
        }
    }
    if ( chess.get_turn() == Chess::BLACK ) {
        hash_key ^= side_key;
    }
    if ( chess.get_castling_rights() & Chess::WHITE_KINGSIDE_CASTLE ) {
        hash_key ^= castling_keys[0];
    }
    if ( chess.get_castling_rights() & Chess::WHITE_QUEENSIDE_CASTLE ) {
        hash_key ^= castling_keys[1];
    }
    if ( chess.get_castling_rights() & Chess::BLACK_KINGSIDE_CASTLE ) {
        hash_key ^= castling_keys[2];
    }
    if ( chess.get_castling_rights() & Chess::BLACK_QUEENSIDE_CASTLE ) {
        hash_key ^= castling_keys[3];
    }
    return hash_key;
}

uint64_t update_hash_key( uint64_t hash_key, const Chess::Move& move, const Chess& chess ) {
    int from = move.from;
    int to = move.to;
    char piece = chess.get_piece( from );
    if ( piece == Chess::EMPTY ) {
        return hash_key;
    }
    int color = chess.get_color( from );
    hash_key ^= piece_keys[color * 6 + piece][from];
    if ( move.promotion != Chess::EMPTY ) {
        char promoted_piece = move.promotion;
        hash_key ^= piece_keys[color * 6 + promoted_piece][to];
    } else {
        hash_key ^= piece_keys[color * 6 + piece][to];
    }
    if ( move.capture != Chess::EMPTY ) {
        int capture_square = chess.get_capture_square( move );
        char captured_piece = move.capture;
        int capture_color = 1 - color;
        hash_key ^= piece_keys[capture_color * 6 + captured_piece][capture_square];
    }
    if ( chess.get_turn() == Chess::WHITE ) {
        hash_key ^= side_key;
    }
    if ( chess.get_castling_rights() & Chess::WHITE_KINGSIDE_CASTLE ) {
        hash_key ^= castling_keys[0];
    }
    if ( chess.get_castling_rights() & Chess::WHITE_QUEENSIDE_CASTLE ) {
        hash_key ^= castling_keys[1];
    }
    if ( chess.get_castling_rights() & Chess::BLACK_KINGSIDE_CASTLE ) {
        hash_key ^= castling_keys[2];
    }
    if ( chess.get_castling_rights() & Chess::BLACK_QUEENSIDE_CASTLE ) {
        hash_key ^= castling_keys[3];
    }
    return hash_key;
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                        USAGE                                                       */
/* ------------------------------------------------------------------------------------------------------------------ */
#include <unordered_map>

#include "chess.h"

std::unordered_map<uint64_t, int> transposition_table;

void search( Chess& chess, int depth ) {
    uint64_t hash_key = get_hash_key( chess );
    if ( transposition_table.count( hash_key ) && transposition_table[hash_key] >= depth ) {
        // This position has already been searched to the desired depth or deeper, so we can use the cached score.
        return;
    }
    if ( depth == 0 ) {
        // We have reached the desired search depth, so evaluate the position and store the score in the transposition
        // table.
        int score = evaluate( chess );
        transposition_table[hash_key] = depth;
        return;
    }
    Chess::MoveList moves = chess.get_legal_moves();
    for ( const auto& move : moves ) {
        // Make the move on the board and update the hash key.
        chess.make_move( move );
        uint64_t new_hash_key = update_hash_key( hash_key, move, chess );
        // Recursively search the resulting position.
        search( chess, depth - 1 );
        // Undo the move and restore the old hash key.
        chess.undo_move();
        hash_key = new_hash_key;
    }
}

int main() {
    Chess chess;
    // Initialize the Zobrist hash table.
    init_zobrist();
    // Perform a search with a depth of 4 plys.
    search( chess, 4 );
    return 0;
}

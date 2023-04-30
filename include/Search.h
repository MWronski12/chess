#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "MoveContent.h"
#include "Movegen.h"

/* ------------------- Minimum and maximum score constants ------------------ */
static const int POSITIVE_INFINITY = std::numeric_limits<int>::max();
static const int NEGATIVE_INFINITY = std::numeric_limits<int>::min();

class TranspositionTable {
private:
    struct TranspositionEntry {
        MoveContent best_move;
        uint64_t hash_key;
        int score;
        int depth;
        int flag;  // alpha/beta etc
        int NodeType;
    };

public:
    TranspositionTable( unsigned int size ) : T( size ) {
        // clear hash table
    }
    TranspositionTable() : T( 10000 ) {
        // clear hash table
    }

    std::vector<std::optional<TranspositionEntry>> T;
    unsigned int index( uint64_t key ) const { return key % T.size(); }
    void putEntry( uint64_t hash, int score, int depth, int flag, MoveContent best_move ) {
        unsigned int i = index( hash );                          // compute the index position
        T[i].emplace( best_move, hash, score, depth, flag, 0 );  // and overide any existing value.
    }
    std::optional<MoveContent> getEntry( uint64_t hash ) {
        unsigned int i = index( hash );  // compute the index position
        if ( T[i].value().hash_key == hash )
            return T[i].value().best_move;  // and return value if key matches
        else
            return std::nullopt;
    }
    /* ---------------------------------------------------- get entry  --------------------------------------------------- */
    // read hash entry data
    // static inline int read_hash_entry( int alpha, int beta, int depth ) {
    //     // create a TT instance pointer to particular hash entry storing
    //     // the scoring data for the current board position if available
    //     tt* hash_entry = &hash_table[hash_key % hash_size];

    //     // make sure we're dealing with the exact position we need
    //     if ( hash_entry->hash_key == hash_key ) {
    //         // make sure that we match the exact depth our search is now at
    //         if ( hash_entry->depth >= depth ) {
    //             // match the exact (PV node) score
    //             if ( hash_entry->flag == hash_flag_exact )
    //                 // return exact (PV node) score
    //                 return hash_entry->score;

    //             // match alpha (fail-low node) score
    //             if ( ( hash_entry->flag == hash_flag_alpha ) && ( hash_entry->score <= alpha ) )
    //                 // return alpha (fail-low node) score
    //                 return alpha;

    //             // match beta (fail-high node) score
    //             if ( ( hash_entry->flag == hash_flag_beta ) && ( hash_entry->score >= beta ) )
    //                 // return beta (fail-high node) score
    //                 return beta;
    //         }
    //     }

    //     // if hash entry doesn't exist
    //     return no_hash_entry;
    // }

    /* --------------------------------------------------- Write entry -------------------------------------------------- */
    // static inline void write_hash_entry( int score, int depth, int hash_flag ) {
    //     // create a TT instance pointer to particular hash entry storing
    //     // the scoring data for the current board position if available
    //     tt* hash_entry = &hash_table[hash_key % hash_size];

    //     // write hash entry data
    //     hash_entry->hash_key = hash_key;
    //     hash_entry->score = score;
    //     hash_entry->flag = hash_flag;
    //     hash_entry->depth = depth;
    // }

};  // zrobic notatke na potem i to stestować

class Search {
public:
    Search() {}
    Search( TranspositionTable& tT ) : tt( tT ) {}
    Search( Search& ) = delete;
    Search( Search&& ) = delete;

    MoveContent getBestMove( Board& board, int maxDepth, bool maximizingPlayer );
    std::vector<MoveContent> evaluateMoves( Board& board );
    TranspositionTable tt;

private:
    mutable PieceValidMoves generator;

    int alphaBeta( Board& board, int depth, int alpha, int beta, bool maximizingPlayer );
    int quiescentSearch( const Board& board, int alpha, int beta, bool maximizingPlayer ) const;

    int endOfTheGameScore( Board& board );
};
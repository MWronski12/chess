#include "TranspositionTable.h"

#include <algorithm>
#include <limits>
#include <optional>

// wywołujemy w root na samym końcu - exact, i w node exact gdy alpha>old_alpha, lower gdy score >= beta, upper gdy nie exact i
// nie lower
void TranspositionTable::putEntry( uint64_t hash, int score, int depth, NodeType NodeType,
                                   std::optional<MoveContent> best_move ) {
    unsigned int i = index( hash );                           // compute the index position
    T[i].emplace( best_move, hash, score, depth, NodeType );  // and overide any existing value.
}

// wywołujemy w root oczekując exact, lub w node oczekując exact lub lower lub upper
std::optional<TranspositionEntry> TranspositionTable::getEntry( uint64_t hash ) {
    unsigned int i = index( hash );  // compute the index position
    if ( T[i].has_value() ) {
        if ( T[i].value().hash_key == hash )
            return T[i].value();  // and return value if key matches
        else
            return std::nullopt;
    }
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
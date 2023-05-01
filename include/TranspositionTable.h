#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <cstdint>
#include <optional>
#include <vector>

#include "Board.h"
#include "MoveContent.h"

enum NodeType { NONE, EXACT, LOWERBOUND, UPPERBOUND };

struct TranspositionEntry {
    std::optional<MoveContent> best_move;
    uint64_t hash_key;
    int score;
    int depth;
    enum NodeType NodeType;  // alpha/beta etc
};
class TranspositionTable {
private:
public:
    TranspositionTable( unsigned int size ) : T( size ) {
        // clear hash table
    }
    TranspositionTable() : T( 50000 ) {
        // clear hash table
    }
    unsigned int index( uint64_t key ) const { return key % T.size(); }
    std::vector<std::optional<TranspositionEntry>> T;
    std::optional<TranspositionEntry> getEntry( uint64_t hash );
    void putEntry( uint64_t hash, int score, int depth, NodeType NodeType, std::optional<MoveContent> best_move );
};  // zrobic notatke na potem i to stestować

#endif
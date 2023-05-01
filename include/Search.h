#ifndef SEARCH_H
#define SEARCH_H

#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "MoveContent.h"
#include "Movegen.h"
#include "TranspositionTable.h"

/* ------------------- Minimum and maximum score constants ------------------ */
static const int POSITIVE_INFINITY = std::numeric_limits<int>::max();
static const int NEGATIVE_INFINITY = std::numeric_limits<int>::min();
class Search {
public:
    Search() = default;
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
#endif
#ifndef SEARCH_H
#define SEARCH_H

#include <algorithm>
#include <limits>
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "MoveContent.h"
#include "Movegen.h"

/* ------------------- Minimum and maximum score constants ------------------ */
static const int POSITIVE_INFINITY = std::numeric_limits<int>::max();
static const int NEGATIVE_INFINITY = std::numeric_limits<int>::min();

class Search {
public:
    Search() {}
    Search( Search& ) = delete;
    Search( Search&& ) = delete;

    MoveContent getBestMove( const Board& examineBoard, int maxDepth, bool maximizingPlayer, int timeout = 5,
                             int nodesExamined = 0, int nodesEvaluated = 0, int nodesPruned = 0 ) const;
    std::vector<MoveContent> getPossibleMoves( const Board& board ) const;

private:
    mutable PieceValidMoves generator;

    int alphaBeta( const Board& examineBoard, int depth, int alpha, int beta, bool maximizingPlayer, int& nodesExamined,
                   int& nodesEvaluated, int& nodesPruned ) const;
    int quiescentSearch( const Board& board, int alpha, int beta, bool maximizingPlayer ) const;

    int endOfTheGameScore( const Board& board ) const;
};

#endif  // SEARCH_H
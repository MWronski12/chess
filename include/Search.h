#include <algorithm>
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

    MoveContent getBestMove( const Board& board, int maxDepth, bool maximizingPlayer ) const;

private:
    mutable PieceValidMoves generator;

    int alphaBeta( const Board& board, int depth, int alpha, int beta, bool maximizingPlayer ) const;
    int quiescentSearch( const Board& board, int alpha, int beta, bool maximizingPlayer ) const;

    std::vector<MoveContent> evaluateMoves( const Board& board ) const;

    // TODO: Implement quiescent search.
    int endOfTheGameScore( const Board& board ) const;
};

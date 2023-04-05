#include <algorithm>
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "MoveContent.h"
#include "Movegen.h"

class Search {
public:
    Search() {}
    Search( Search& ) = delete;
    Search( Search&& ) = delete;

    MoveContent getBestMove( const Board& board, int depth );
    int miniMax( int depth, const Board& board );
    // int quiescence( Board& board ) {}
    std::vector<MoveContent> evaluateMoves( const Board& board );

private:
    PieceValidMoves generator;
    MoveContent miniMaxMove( PieceColor sideToMove, const std::vector<MoveContent>& moves );
    int miniMaxScore( PieceColor sideToMove, const std::vector<MoveContent>& moves );
};

<<<<<<< HEAD
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "Move.h"
#include "Search.h"

MoveContent Search::getBestMove(Board board) {
  MoveContent move;
  int evaluation = Evaluation::evaluateBoard(board);

  return move;
}

int Search::miniMaxAB(Board board, int depth, int alpha, int beta) {}
std::vector<MoveContent> Search::evaluteMoves(Board board) {}
void Search::searchForMate(Board board) {}
int Search::quiescence(Board board, int alpha, int beta) {}
=======
#include <vector>

#include "Board.h"
#include "Evaluation.h"
#include "Move.h"
#include "Search.h"

MoveContent Search::getBestMove( Board board ) {
    MoveContent move;
    int evaluation = Evaluation::evaluateBoard( board );

    return move;
}

int Search::miniMaxAB( Board board, int depth, int alpha, int beta ) {}
std::vector<MoveContent> Search::evaluteMoves( Board board ) {}
void Search::searchForMate( Board board ) {}
int Search::quiescence( Board board, int alpha, int beta ) {}
>>>>>>> f1c2414a580b491c8cceeeeb44d4601a35dfc311

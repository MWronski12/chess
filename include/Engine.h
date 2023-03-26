#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"
#include "Move.h"
#include "Movegen.h"

class Engine {
public:
  Engine();
  Board board;
  std::stack<MoveContent> moveHistory;

private:
  PieceMovesIterator movesIterator;
  PieceValidMoves validMoves;
};

#endif
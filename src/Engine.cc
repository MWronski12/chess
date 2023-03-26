#include "Engine.h"

Engine::Engine() {
  movesIterator = new PieceMoves();
  validMoves = new PieceValidMoves(moveIterator);
}

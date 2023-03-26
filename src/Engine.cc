/*
 * Brief: Engine class whose responsibility is to use all classes to create chess engine
 *
 * Author: Jakub Dydynski
 * Date: 26.03.2023
 */

#include "Engine.h"

Engine::Engine(PieceMovesNestedLists &PieceMoves) {
  movesIterator = PieceMoves;
  validMoves = PieceValidMoves(movesIterator);
}

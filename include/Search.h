#ifndef SEARCH_H
#define SEARCH_H

class Search {
public:
  MoveContent getBestMove(Board board);

private:
  int miniMaxAB(Board board, int depth, int alpha, int beta);
  std::vector<MoveContent> evaluteMoves(Board board);
  void searchForMate(Board board);
  int quiescence(Board board, int alpha, int beta);
};

#endif
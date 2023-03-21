#include <iostream>

#include "gui.hpp"

using namespace std;

int main() {
  cout << "Starting new game of chess...\n\n";
  ConsoleGui gui;

  while (true) {
    int src;
    int dest;
    int promotion;

    cin >> src;
    cin >> dest;
    cin >> promotion;

    try {
      gui.makeMove(src, dest, (PieceType)promotion);
    } catch (exception &e) {
      cout << "Bad move input!\n";
      continue;
    }
  }

  return 1;
}

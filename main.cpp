#include <iostream>

#include "Gui.hpp"
#include "Movegen.h"

using namespace std;

int main() {
    PieceMoves pm = PieceMoves::getInstance();

    for ( auto &ray : pm.getMoveList( WHITE, ROOK, 0 ) ) {
        cout << "ray: ";
        for ( auto &move : ray ) {
            cout << (int)move << " ";
            if ( move == 24 ) {
                cout << "skipping";
                break;
            }
        }
        cout << "\n";
    }

    // cout << "Starting new game of chess...\n";
    // ConsoleGui gui;

    // while ( true ) {
    //     std::string input;
    //     cin >> input;

    //     try {
    //         gui.makeMove( input );
    //     } catch ( exception &e ) {
    //         cout << e.what();
    //         continue;
    //     }
    // }

    return 1;
}

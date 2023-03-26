#include <iostream>

#include "Gui.hpp"

using namespace std;

int main() {
    cout << "Starting new game of chess...\n";
    ConsoleGui gui;

    while ( true ) {
        std::string input;
        cin >> input;

        try {
            gui.makeMove( input );
        } catch ( exception &e ) {
            cout << e.what();
            continue;
        }
    }

    return 1;
}

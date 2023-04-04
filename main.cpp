#include <iostream>

#include "Gui.hpp"

using namespace std;

int main() {
    Piece p = Piece( 'p' );
    cout << sizeof p;
    // Gui *gui = new ConsoleGui();
    // gui->start();

    return 0;
}

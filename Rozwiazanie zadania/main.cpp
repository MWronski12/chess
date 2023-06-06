#include <iostream>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    WindowGui gui = WindowGui();  // Window gui does not integrate the engine at this point.
    // ConsoleGui gui = ConsoleGui(); // Console gui integrates the engine and provides info on the best move.
    gui.start();
    return 0;
}

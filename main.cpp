#include <iostream>

#include "Engine.h"
#include "Gui.hpp"
#include "Search.h"

using namespace std;

int main() {
    // Gui *gui = new ConsoleGui();
    // gui->start();
    WindowGui gui = WindowGui();
    gui.start();
    return 0;
}

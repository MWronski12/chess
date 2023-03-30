#include <iostream>

#include "Gui.hpp"

using namespace std;

int main() {
    Gui* gui = new ConsoleGui();

    gui->start();

    return 0;
}

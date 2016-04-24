#include "game.h"
#include "cli.h"
#include <iostream>
using namespace std;
using namespace minesweeperCLI;

int main(int argc, char** argv) {
    Game game;
    game.start();

    return 0;
}

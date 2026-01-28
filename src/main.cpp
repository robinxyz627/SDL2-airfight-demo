#include <iostream>
#include "game.h"
#include "sceneMain.h"

int main(int, char **)
{
    Game& game = Game::getInstance();
    game.init();
    game.run();
    // game.changeScene(sceneMain);
    return 0;
}
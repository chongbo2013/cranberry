#include <Cranberry/Game/Game.hpp>
#include <GameWindow.hpp>


int main(int argc, char *argv[])
{
    Game game(argc, argv);
    GameWindow gameWindow;

    return game.run(&gameWindow);
}

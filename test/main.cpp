#include <Cranberry/Game/Game.hpp>
#include <QCoreApplication>
#include "Test.hpp"


int main(int argc, char *argv[])
{
    cran::Game game(argc, argv);
    int32_t result = game.run(new Test());

    return result;
}

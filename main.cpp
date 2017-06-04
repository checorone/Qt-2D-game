#include <QGuiApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Game game;
    game.startGame();
    return app.exec();
}

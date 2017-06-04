#ifndef GAME_H
#define GAME_H
#include <QObject>
#include <QQmlApplicationEngine>

#include "fish.h"
#include "worm.h"
#include "booster.h"




class Game : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int points READ getPoints NOTIFY statChanged)
    Q_PROPERTY(int bestpoints READ getBestPoints NOTIFY statChanged)
    Q_PROPERTY(int deaths READ getDeaths() NOTIFY statChanged)

private:

    Fish * fish;
    Worm * worm;
    Booster * booster;

    QQmlApplicationEngine * engine;

private:
    bool started;
    int points;
    int bestpoints;
    int deaths;

private slots:
    void init();
    void render();
    void shutdown();

    void update();

public slots:
    void onMouseClicked(double x, double y);
    void onUpdated(double x, double y);

public:
    Game();
    ~Game();

public:
    void startGame();

    int getPoints() const { return points; }
    int getBestPoints() const { return bestpoints; }
    int getDeaths() const { return deaths; }




signals:
    void close();
    void start();

    void statChanged();

};

#endif // GAME_H

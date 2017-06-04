#include "game.h"
#include <QQmlContext>
#include <QTimer>

Game::Game() : QObject(), started(0), points(0), bestpoints(0), deaths(0)
{
    connect(this, SIGNAL(start()), this, SLOT(init()));
    connect(this, SIGNAL(close()), this, SLOT(shutdown()));

    fish = new Fish(300,300, 1, 10);
    fish->setDestPoint(300,150);
    worm = new Worm(0,0,4,3);
    booster = new Booster(0,0,0);

    engine = new QQmlApplicationEngine(this);

}

Game::~Game()
{
    delete fish;
    delete worm;
    delete booster;

    delete engine;
}

void Game::startGame()
{
    init();
}

void Game::init()
{
    engine->rootContext()->setContextProperty("fish", fish);
    engine->rootContext()->setContextProperty("worm", worm);
    engine->rootContext()->setContextProperty("booster", booster);
    engine->rootContext()->setContextProperty("game", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

    //TODO: connect signals
    connect(engine->rootObjects()[0], SIGNAL(mouseClicked(double,double)), this, SLOT(onMouseClicked(double,double)));
    connect(engine->rootObjects()[0], SIGNAL(updated(double,double)), this, SLOT(onUpdated(double,double)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(render()));
    timer->start(30);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(update()));
    timer2->start(1000);
}

void Game::render()
{
        if(started)
        {
            if(fish->check())
            {
                started = 0;
                points = 0;
                deaths++;
                worm->toDefault();
                booster->toDefault();
                fish->toDefault();

                emit statChanged();
            }
            else
            {
                if(worm->check(booster->getX(), booster->getY()))
                {
                    worm->setBoost(worm->getBoost() + booster->getValue());
                    worm->setTime(15);
                    booster->toDefault();
                    booster->setXY(rand() % 600, rand() % 600);
                    booster->setValue(1);
                    booster->reset();

                    points += 100;
                    if(points >= bestpoints)
                        bestpoints += 100;
                }
                if(worm->check())
                {

                }
                else
                {
                    worm->move();
                    fish->setDestPoint(worm->getX(), worm->getY());
                }
                fish->move();

                emit statChanged();
            }
        }
        else
        {
            if(fish->check())
                fish->setDestPoint(rand() % 600, rand() % 600);
            else
                fish->move();
        }
}

void Game::shutdown()
{

}

void Game::update()
{
    if(started)
    {
        points += fish->getVelocity() * 10;
        if(points >= bestpoints)
            bestpoints += fish->getVelocity() * 10;
        fish->setVelocity(fish->getVelocity() + 0.1);
        if(worm->getTime() > 0)
            worm->setTime(worm->getTime() - 1);
        else
            worm->setBoost(0);
    }

}

void Game::onMouseClicked(double x, double y)
{
    if(!started)
    {
        started = true;
        worm->setXY(x,y);
        worm->setDestPoint(x,y);
        worm->reset();
        booster->setXY(rand() % 600, rand() % 600);
        booster->setValue(1);
        booster->reset();
        fish->setDestPoint(worm->getX(), worm->getY());
    }
}

void Game::onUpdated(double x, double y)
{
    worm->setDestPoint(x, y);
}





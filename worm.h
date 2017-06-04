#ifndef WORM_H
#define WORM_H
#include <QObject>
#include"movableobject.h"

class Worm : public MovableObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ activated)
    Q_PROPERTY(int boost READ getBoost NOTIFY boostChanged)
    Q_PROPERTY(int time READ getTime NOTIFY boostChanged)
private:
    int boostedVelo;
    int boostedtime;

    bool active;


public:
    Worm(double x_, double y_, double velo, int size_)
        : MovableObject(x_, y_, velo, size_)
        , boostedVelo(0), boostedtime(0), active(0) {}
    ~Worm() {}

public:

    int getBoost() const { return boostedVelo; }
    int getTime() const { return boostedtime; }
    bool activated() const { return active; }

    void setBoost(int boost) { setVelocity(getVelocity() - boostedVelo); boostedVelo = boost; setVelocity(getVelocity() + boostedVelo); emit boostChanged(); }
    void setTime(int time) { boostedtime = time; emit boostChanged(); }
    void reset() { active = !active; }

public:
    bool check(double x, double y) const;
    bool check() const { return MovableObject::check(); }
    void toDefault() { MovableObject::toDefault(0,0,3,4); boostedVelo = boostedtime = active = 0; }

signals:
    void boostChanged();

};

#endif // WORM_H

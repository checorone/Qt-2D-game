#ifndef FISH_H
#define FISH_H
#include<QObject>
#include"movableobject.h"

class Fish : public MovableObject
{
    Q_OBJECT

private:

public:
    Fish(double x_, double y_, double velo, int size_) : MovableObject(x_, y_, velo, size_) {}
     ~Fish() {}
public:
    void toDefault() { MovableObject::toDefault(getX(),getY(),1,10); }

};

#endif // FISH_H

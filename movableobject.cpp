#include "movableobject.h"
#include <QVector2D>


MovableObject::MovableObject(double x_, double y_, double velo, int size_)
    : QObject()
    , x(x_), y(y_)
    , velocity(velo), size(size_), rotationAngle(0), absoleteAngle(0), orientation(right)
{
    connect(this, SIGNAL(stateChanged()), this, SLOT(updateMatrix()));

    coords(0,0) = x - size * 4;
    coords(0,1) = y - size * 3;
    coords(0,2) = 1;
    coords(1,0) = x - size * 4;
    coords(1,1) = y + size * 3;
    coords(1,2) = 1;
    coords(2,0) = x + size * 5;
    coords(2,1) = y;
    coords(2,2) = 1;

    updateMatrix();

    setDestPoint(x_,y_);


}

QList<double> MovableObject::getCoords() const
{
    QList<double> list;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            list.append(coords(i,j));
    return list;
}

void MovableObject::setXY(double x_, double y_)
{
    x = x_;
    y = y_;

    coords(0,0) = x - size * 4;
    coords(0,1) = y - size * 3;
    coords(0,2) = 1;
    coords(1,0) = x - size * 4;
    coords(1,1) = y + size * 3;
    coords(1,2) = 1;
    coords(2,0) = x + size * 5;
    coords(2,1) = y;
    coords(2,2) = 1;

    updateMatrix();
}


void MovableObject::updateMatrix()
{
    rotateMatrix(0,0) = cos(rotationAngle);
    rotateMatrix(0,1) = sin(rotationAngle);
    rotateMatrix(0,2) = 0;
    rotateMatrix(1,0) = -sin(rotationAngle);
    rotateMatrix(1,1) = cos(rotationAngle);
    rotateMatrix(1,2) = 0;
    rotateMatrix(2,0) = 0;
    rotateMatrix(2,1) = 0;
    rotateMatrix(2,2) = 1;

    reflectXMatrix(0,0) = -1;
    reflectXMatrix(0,1) = 0;
    reflectXMatrix(0,2) = 0;
    reflectXMatrix(1,0) = 0;
    reflectXMatrix(1,1) = 1;
    reflectXMatrix(1,2) = 0;
    reflectXMatrix(2,0) = 0;
    reflectXMatrix(2,1) = 0;
    reflectXMatrix(2,2) = 1;

    transformToMatrix(0,0) = 1;
    transformToMatrix(0,1) = 0;
    transformToMatrix(0,2) = 0;
    transformToMatrix(1,0) = 0;
    transformToMatrix(1,1) = 1;
    transformToMatrix(1,2) = 0;
    transformToMatrix(2,0) = -getX();
    transformToMatrix(2,1) = -getY();
    transformToMatrix(2,2) = 1;

    transformBackMatrix(0,0) = 1;
    transformBackMatrix(0,1) = 0;
    transformBackMatrix(0,2) = 0;
    transformBackMatrix(1,0) = 0;
    transformBackMatrix(1,1) = 1;
    transformBackMatrix(1,2) = 0;
    transformBackMatrix(2,0) = getX();
    transformBackMatrix(2,1) = getY();
    transformBackMatrix(2,2) = 1;

    reflectXMatrix = transformToMatrix * reflectXMatrix * transformBackMatrix;
    rotateMatrix = transformToMatrix * rotateMatrix * transformBackMatrix;

    moveMatrix(0,0) = 1;
    moveMatrix(0,1) = 0;
    moveMatrix(0,2) = 0;
    moveMatrix(1,0) = 0;
    moveMatrix(1,1) = 1;
    moveMatrix(1,2) = 0;
    moveMatrix(2,0) = dX();
    moveMatrix(2,1) = dY();
    moveMatrix(2,2) = 1;

}


void MovableObject::setDestPoint(double x_, double y_)
{
    destX = x_;
    destY = y_;

    rotationAngle = -rotationAngle;
    updateMatrix();
    coords = coords * rotateMatrix;

    bool reflect = false;
    QVector2D v1(destX - coords(1,0), destY - coords(2,1));
    QVector2D v2(destX - coords(2,0), destY - coords(2,1));
    if( sqrt( v1.x() * v1.x() ) < sqrt( v2.x() * v2.x()) * (size >= 10 ? 0.8 : 1) )
         reflect = true;

    double oposite = 0;
    double near = 1;
    if( destX > x && destY < y)
    {
        oposite = destY - y;
        near = destX - x;
        absoleteAngle = qAtan(oposite/near);
        rotationAngle = absoleteAngle;
    }

    if( destX < x && destY < y)
    {
        oposite = destY - y;
        near = destX - x;
        absoleteAngle = -M_PI + qAtan(oposite/near);
        rotationAngle = -M_PI + absoleteAngle;
    }

    if( destX < x && destY > y)
    {
        oposite = destY - y;
        near = destX - x;
        absoleteAngle = M_PI + qAtan(oposite/near);
        rotationAngle = M_PI + absoleteAngle;
    }

    if( destX > x && destY > y)
    {
        oposite = destY - y;
        near = destX - x;
        absoleteAngle = qAtan(oposite/near);
        rotationAngle = absoleteAngle;
    }

    if( destX >= x * 0.99  && destX <= x * 1.01 && destY > y)
    {
        absoleteAngle = M_PI / 2;
        rotationAngle = absoleteAngle;
        reflect = false;
        if(orientation == left)
        {
            coords = coords * reflectXMatrix;
            reOrient();
        }
    }

    if( destX >= x * 0.99 && destX <= x * 1.01 && destY < y)
    {
        absoleteAngle = -M_PI / 2;
        rotationAngle = absoleteAngle;
        reflect = false;
        if(orientation == left)
        {
            coords = coords * reflectXMatrix;
            reOrient();
        }
    }
    updateMatrix();



    if( reflect )
    {
        coords = coords * reflectXMatrix;
        reOrient();
    }

    coords = coords * rotateMatrix;

    emit coordsChanged();
}

void MovableObject::move()
{
    x += dX();
    y += dY();

    updateMatrix();

    coords = coords * moveMatrix;

    emit coordsChanged();
}

bool MovableObject::check() const
{
    QVector2D v1(coords(0,0) - destX, coords(0,1) - destY);
    QVector2D v2(coords(1,0) - destX, coords(1,1) - destY);
    QVector2D v3(coords(2,0) - destX, coords(2,1) - destY);

    QVector2D a(coords(0,0) - coords(1,0), coords(0,1) - coords(1,1));
    QVector2D b(coords(1,0) - coords(2,0), coords(1,1) - coords(2,1));
    QVector2D c(coords(2,0) - coords(0,0), coords(2,1) - coords(0,1));

    double p123 = v1.length() + v2.length() + v3.length();
    double pabc = a.length() + b.length() + c.length();

    if(p123 <= pabc * 0.8)
        return true;
    else
        return false;
}

void MovableObject::toDefault(double x_, double y_, double velo, int size_)
{
    x = x_;
    y = y_;
    destX = x;
    destY = y;
    velocity = velo;
    size = size_;
    rotationAngle = 0;
    absoleteAngle = 0;
    orientation = right;

    coords(0,0) = x - size * 4;
    coords(0,1) = y - size * 3;
    coords(0,2) = 1;
    coords(1,0) = x - size * 4;
    coords(1,1) = y + size * 3;
    coords(1,2) = 1;
    coords(2,0) = x + size * 5;
    coords(2,1) = y;
    coords(2,2) = 1;

    updateMatrix();

}

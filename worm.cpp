#include "worm.h"
#include <QVector2D>

bool Worm::check(double x, double y) const
{
    QVector2D v1(coords(0,0) - x, coords(0,1) - y);
    QVector2D v2(coords(1,0) - x, coords(1,1) - y);
    QVector2D v3(coords(2,0) - x, coords(2,1) - y);

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

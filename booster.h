#ifndef BOOSTER_H
#define BOOSTER_H
#include <QObject>


class Booster : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double x READ getX )
    Q_PROPERTY(double y READ getY )
    Q_PROPERTY(bool active READ activated )
private:
    double x, y;

    double value;

    bool active;
public:
    Booster(double x_, double y_, double value_) : QObject(), x(x_), y(y_), value(value_), active(0) {}

public:
    double getX() const { return x; }
    double getY() const { return y; }
    double getValue() const { return value; }
    bool activated() const { return active; }

    void setXY(double x_, double y_) { x = x_; y = y_; }
    void setValue(double value_) { value = value_; }
    void reset() { active = !active; }
public:
    void toDefault() {  x = y = value = active = 0; }
};

#endif // BOOSTER_H

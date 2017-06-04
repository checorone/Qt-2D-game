#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H


#include<QObject>
#include<QMatrix3x3>
#include<QtMath>
#include<QList>


class MovableObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<double> coords READ getCoords)
    Q_PROPERTY(double x READ getX)
    Q_PROPERTY(double y READ getY)
    Q_PROPERTY(double destX READ getDestX)
    Q_PROPERTY(double destY READ getDestY)
    Q_PROPERTY(double rotationAngle READ getRotationAngle)
    Q_PROPERTY(double absoleteAngle READ getAbsoleteAngle)

private:
    QMatrix3x3 moveMatrix;
    QMatrix3x3 transformToMatrix;
    QMatrix3x3 transformBackMatrix;
    QMatrix3x3 rotateMatrix;
    QMatrix3x3 reflectXMatrix;
    QMatrix3x3 reflectYMatrix;

    double x, y;

    double velocity;

    int size;

    double destX, destY;

    double rotationAngle;
    double absoleteAngle;

    enum state { left, right };
    state orientation;


private slots:
    void updateMatrix();

protected:
    QMatrix3x3 coords;
    double dX() const { return velocity * cos(absoleteAngle); }
    double dY() const { return velocity * sin(absoleteAngle); }

public:
    MovableObject(double, double, double, int);
    ~MovableObject() {}

public:
    QList<double> getCoords() const;
    double getX() const { return x; }
    double getY() const { return y; }
    double getVelocity() const { return velocity; }
    double getDestX() const { return destX; }
    double getDestY() const { return destY; }
    double getRotationAngle() const { return rotationAngle; }
    double getAbsoleteAngle() const { return absoleteAngle; }


    void setXY(double x_, double y_);
    void setVelocity(double velo) { velocity = velo; updateMatrix(); }
    void setDestPoint(double x_, double y_);
    void reOrient() { if(orientation == left) orientation = right; else orientation = left; }

public:
    void move();
    bool check() const;
    void toDefault(double x_, double y_, double velo, int size_);

signals:
    void coordsChanged();
    void stateChanged();

};

#endif // MOVABLEOBJECT_H

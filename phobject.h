#ifndef PHOBJECT_H
#define PHOBJECT_H

#include <QColor>

class PhObject
{
private:
    double x, y;
    double xSpeed, ySpeed;
    double mass;
    double radius;
    double q;
    QColor color;

public:
    PhObject(double mass = 0, double rad = 1, double q = 0, double x = 0, double y = 0, double xS = 0, double yS = 0);

    //----Setters----
    void setPosition (double x, double y); //Position methods
    void setXPosition(double x          );
    void setYPosition(double y          );

    void setSpeed (double xS, double yS); //Speed methods
    void setXSpeed(double xS           );
    void setYSpeed(double yS           );

    void setMass  (double mass  );
    void setRadius(double radius);
    void setQ(double q);
    void setColor (QColor color );


    //----Getters----
    double getXPosition();
    double getYPosition();

    double getXSpeed();
    double getYSpeed();

    double getMass();
    double getRadius();
    double getQ();
    QColor getColor();
};

#endif // PHOBJECT_H

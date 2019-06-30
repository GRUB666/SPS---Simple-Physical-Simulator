#include "phobject.h"

PhObject::PhObject(long double mass, long double rad, long double q, long double x, long double y, long double xS, long double yS)
{
    this->mass = mass;
    this->radius = rad;
    this->q = q;
    this->x = x;
    this->y = y;
    this->xSpeed = xS;
    this->ySpeed = yS;
    this->color = Qt::black;
    this->Name = "Object";
    this->isStatic = false;
    this->inFocus = false;
    xAccel = yAccel = 0;
}

void PhObject::setPosition(long double x, long double y)
{
    this->x = x;
    this->y = y;
}

void PhObject::setXPosition(long double x)
{
    this->x = x;
}

void PhObject::setYPosition(long double y)
{
    this->y = y;
}

void PhObject::setSpeed(long double xS, long double yS)
{
    this->xSpeed = xS;
    this->ySpeed = yS;
}

void PhObject::setXSpeed(long double xS)
{
    this->xSpeed = xS;
}

void PhObject::setYSpeed(long double yS)
{
    this->ySpeed = yS;
}

void PhObject::setMass(long double mass)
{
    this->mass = mass;
}

void PhObject::setRadius(long double radius)
{
    this->radius = radius;
}

void PhObject::setQ(long double q)
{
    this->q = q;
}

void PhObject::setColor(QColor color)
{
    this->color = color;
}

void PhObject::setName(QString name)
{
    this->Name = name;
}

void PhObject::setStatic(bool val)
{
    this->isStatic = val;
}

void PhObject::setFocus(bool val)
{
    this->inFocus = val;
}

long double PhObject::getXPosition()
{
    return x;
}

long double PhObject::getYPosition()
{
    return y;
}

long double PhObject::getXSpeed()
{
    return xSpeed;
}

long double PhObject::getYSpeed()
{
    return ySpeed;
}

long double PhObject::getMass()
{
    return mass;
}

long double PhObject::getRadius()
{
    return radius;
}

long double PhObject::getQ()
{
    return q;
}

QColor PhObject::getColor()
{
    return color;
}

QString PhObject::getName()
{
    return Name;
}

bool PhObject::getStatic()
{
    return isStatic;
}

bool PhObject::getFocus()
{
    return inFocus;
}

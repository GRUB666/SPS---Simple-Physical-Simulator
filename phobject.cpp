#include "phobject.h"

PhObject::PhObject(double mass, double rad, double q, double x, double y, double xS, double yS)
{
    this->mass = mass;
    this->radius = rad;
    this->q = q;
    this->x = x;
    this->y = y;
    this->xSpeed = xS;
    this->ySpeed = yS;
    this->color = Qt::blue;
    this->Name = "Object";
    this->isStatic = false;
    this->inFocus = false;
}

void PhObject::setPosition(double x, double y)
{
    this->x = x;
    this->y = y;
}

void PhObject::setXPosition(double x)
{
    this->x = x;
}

void PhObject::setYPosition(double y)
{
    this->y = y;
}

void PhObject::setSpeed(double xS, double yS)
{
    this->xSpeed = xS;
    this->ySpeed = yS;
}

void PhObject::setXSpeed(double xS)
{
    this->xSpeed = xS;
}

void PhObject::setYSpeed(double yS)
{
    this->ySpeed = yS;
}

void PhObject::setMass(double mass)
{
    this->mass = mass;
}

void PhObject::setRadius(double radius)
{
    this->radius = radius;
}

void PhObject::setQ(double q)
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



double PhObject::getXPosition()
{
    return x;
}

double PhObject::getYPosition()
{
    return y;
}

double PhObject::getXSpeed()
{
    return xSpeed;
}

double PhObject::getYSpeed()
{
    return ySpeed;
}

double PhObject::getMass()
{
    return mass;
}

double PhObject::getRadius()
{
    return radius;
}

double PhObject::getQ()
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

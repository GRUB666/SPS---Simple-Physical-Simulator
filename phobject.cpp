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

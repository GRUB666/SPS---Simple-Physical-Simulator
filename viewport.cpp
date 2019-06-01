#include "viewport.h"

void Viewport::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setWindow(-width()/2, height()/2, width(), -height());//Decart SO
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    p.setBrush(QBrush(background_color, Qt::SolidPattern));
    p.drawRect(QRect(QPoint(-width()/2, height()/2), QPoint(width()/2-2, -height()/2)));

    for(auto &var : ToPaintVector)
    {
        if(var.getFocus())
            p.setPen(QPen(Qt::darkMagenta, 2, Qt::SolidLine));
        else
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));

        p.setBrush(QBrush(var.getColor(), Qt::SolidPattern));
        p.drawEllipse(var.getXPosition() - var.getRadius() - camX, var.getYPosition() - var.getRadius() - camY, var.getRadius()*2, var.getRadius()*2);
    }
}


Viewport::Viewport(QWidget *parent) : QFrame(parent)
{
    background_color = Qt::white;
    camX = camY = 0;
}

int Viewport::getCamX()
{
    return camX;
}

int Viewport::getCamY()
{
    return camY;
}

void Viewport::setCamX(int xp)
{
    this->camX = xp;
}

void Viewport::setCamY(int yp)
{
    this->camY = yp;
}

void Viewport::setPaintVector(QVector<PhObject> vec)
{
    ToPaintVector = vec;
}

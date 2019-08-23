#include "viewport.h"

void Viewport::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setWindow(-width()/2, height()/2, width(), -height());//Decart SO
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    p.setBrush(QBrush(background_color, Qt::SolidPattern));
    p.drawRect(QRect(QPoint(-width()/2, height()/2), QPoint(width()/2-2, -height()/2)));

    p.scale(1/distance_scale, 1/distance_scale);
    //p.rotate(alpha);

    //Проверка на выход камеры за пределы пространства
    if(camX < -MAX_INT_VALUE)
        camX = MAX_INT_VALUE;
    if(camY < -MAX_INT_VALUE)
        camY = MAX_INT_VALUE;

    if(camX > MAX_INT_VALUE)
        camX = -MAX_INT_VALUE;
    if(camY > MAX_INT_VALUE)
        camY = -MAX_INT_VALUE;



    for(auto &var : ToPaintVector)
    {
        if(var.getFocus())
            p.setPen(QPen(Qt::darkMagenta, 2 * distance_scale, Qt::SolidLine));
        else
            p.setPen(QPen(Qt::black, 1 * distance_scale, Qt::SolidLine));

        p.setBrush(QBrush(var.getColor(), Qt::SolidPattern));
        p.drawEllipse(var.getXPosition() - var.getRadius()*10 - camX, var.getYPosition() - var.getRadius()*10 - camY, var.getRadius()*20, var.getRadius()*20);
    }

    p.setPen(QPen(Qt::magenta, 10, Qt::SolidLine));
    p.drawLine(QPoint(-MAX_INT_VALUE - camX, MAX_INT_VALUE - camY), QPoint(-MAX_INT_VALUE - camX, -MAX_INT_VALUE - camY));
    p.drawLine(QPoint(-MAX_INT_VALUE - camX, MAX_INT_VALUE - camY), QPoint(MAX_INT_VALUE - camX, MAX_INT_VALUE - camY));
    p.drawLine(QPoint(-MAX_INT_VALUE - camX, -MAX_INT_VALUE - camY), QPoint(MAX_INT_VALUE - camX, -MAX_INT_VALUE - camY));
    p.drawLine(QPoint(MAX_INT_VALUE - camX, MAX_INT_VALUE - camY), QPoint(MAX_INT_VALUE - camX, -MAX_INT_VALUE - camY));
}

void Viewport::wheelEvent(QWheelEvent *e)
{
    distance_scale -= SCROLL_SPEED * e->delta()/abs(e->delta());
    if(distance_scale < MIN_DISTANCE_SCALE)
        distance_scale = MIN_DISTANCE_SCALE;
    if(distance_scale > MAX_DISTANCE_SCALE)
        distance_scale = MAX_DISTANCE_SCALE;

    emit whellScrolled(distance_scale*100);
    update();
}

void Viewport::mouseMoveEvent(QMouseEvent *p)
{
    if(p->buttons() != Qt::RightButton)
    {
        camX -= (p->x() - bufferX)*distance_scale;
        camY += (p->y() - bufferY)*distance_scale;

        update();
        emit camScrolled();
        bufferX = p->x();
        bufferY = p->y();
    }
}

void Viewport::mousePressEvent(QMouseEvent *p)
{
    setFocus();

    if(p->button() == Qt::RightButton)
    {
        camX += distance_scale*(p->x() - this->width()/2);
        camY -= distance_scale*(p->y() - this->height()/2);

        this->cursor().setPos(mapToGlobal(QPoint(this->width()/2, this->height()/2)));

        emit replaceCamSignal();
    }

    if(p->button() != Qt::RightButton)
    {
        bufferX = p->x();
        bufferY = p->y();
    }
}


Viewport::Viewport(QWidget *parent) : QFrame(parent)
{
    background_color = Qt::white;
    camX = camY = 0;
    SCROLL_SPEED = 0.1;
    distance_scale = 1;
    MAX_DISTANCE_SCALE = 10;
    MIN_DISTANCE_SCALE = 0.1;

    alpha = 0;
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
    update();
}

void Viewport::setCamY(int yp)
{
    this->camY = yp;
    update();
}

void Viewport::setCamPos(int xp /*= 0*/, int yp/*= 0*/)
{
    this->camX = xp;
    this->camY = yp;
    update();
}

void Viewport::setScrollSpeed(double val)
{
    SCROLL_SPEED = val;
}

void Viewport::setScale(double val)
{
    this->distance_scale = val;
    update();
}

double Viewport::getScrollSpeed()
{
    return SCROLL_SPEED;
}

double Viewport::getScale()
{
    return distance_scale;
}

void Viewport::setAlpha(int alpha)
{
    this->alpha = alpha;
}

void Viewport::setBackgroundColor(QColor color)
{
    background_color = color;
}

double *Viewport::getPointerCamX()
{
    return &camX;
}

double *Viewport::getPointerCamY()
{
    return &camY;
}

void Viewport::setPaintVector(QVector<PhObject>& vec)
{
    ToPaintVector = vec;
}

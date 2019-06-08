#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>
#include <QVector>
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>

#include "phobject.h"

class Viewport : public QFrame
{
    Q_OBJECT
private:
    QVector<PhObject> ToPaintVector;
    QColor background_color;
    double camX, camY;
    double distance_scale;
    int MAX_DISTANCE_SCALE;
    int MIN_DISTANCE_SCALE;
    double SCROLL_SPEED;
    int CAM_SCROLL_SPEED;
    int bufferX, bufferY;
    double sim_speed;
    int alpha;


protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *p);
    virtual void mousePressEvent(QMouseEvent* p);

public:

    explicit Viewport(QWidget *parent = nullptr);

    int getCamX();
    int getCamY();

    void setCamX(int xp);
    void setCamY(int yp);
    void setCamPos(int xp = 0, int yp = 0);
    void setScrollSpeed(double val);
    void setScale(double val);
    double getScrollSpeed();
    double getScale();
    void setAlpha(int alpha);


    void setPaintVector(QVector<PhObject>& vec);

signals:
    void whellScrolled(int value);
    void camScrolled(int x, int y);
};

#endif // VIEWPORT_H

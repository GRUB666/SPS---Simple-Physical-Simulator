#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QPainter>
#include <QVector>
#include <QMouseEvent>

#include "phobject.h"

class Viewport : public QFrame
{
    Q_OBJECT
private:
    QVector<PhObject> ToPaintVector;
    QColor background_color;
    int camX, camY;

protected:
    virtual void paintEvent(QPaintEvent*);

public:
    explicit Viewport(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // VIEWPORT_H

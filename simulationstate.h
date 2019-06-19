#ifndef SIMULATIONSTATE_H
#define SIMULATIONSTATE_H

#include <QColor>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "settings.h"
#include <phobject.h>

class SimulationState
{
private:
    CollisionMode* collision_mode;
    double *G, *K;
    double *CamX, *CamY;
    QColor *background_color;
    QVector<PhObject>* Objects;
    QString file_way;

public:
    SimulationState();
    SimulationState(double *G, double *K, CollisionMode *CM, QColor *BC, double *CX, double *CY, QVector<PhObject> *vec);

    int getColorCode(PhObject& obj);
    int getBackGroundColorCode();

    bool WriteDataInFile(QString way /*= ""*/);
    bool ReadDataFromFile(QString way /*= ""*/);

    void setFileWay(QString &way);
    void setBackgroundColor(int code);
    void setCollisionMode(int mode);

    CollisionMode* getCollisionModePointer(){ return collision_mode; }
    QColor* getBackcolorPointer(){ return background_color; }
    double* getGPointer() { return G; }
    double* getKPointer() { return K; }

private:
    void setColor(int code, PhObject& obj);

};


#endif // SIMULATIONSTATE_H

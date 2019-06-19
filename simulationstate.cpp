#include "simulationstate.h"


SimulationState::SimulationState()
{

}

SimulationState::SimulationState(double *G, double *K, CollisionMode *CM, QColor *BC, double *CX, double *CY, QVector<PhObject> *vec)
{
    this->G = G;
    this->K = K;
    this->CamX = CX;
    this->CamY = CY;
    this->collision_mode = CM;
    this->background_color = BC;
    this->Objects = vec;
    this->file_way = "";
}

int SimulationState::getColorCode(PhObject& obj)
{
    if(obj.getColor() == Qt::black)
        return 0;
    if(obj.getColor() == Qt::red)
        return 1;
    if(obj.getColor() == Qt::green)
        return 2;
    if(obj.getColor() == Qt::blue)
        return 3;
    if(obj.getColor() == Qt::yellow)
        return 4;

    return 0;
}

int SimulationState::getBackGroundColorCode()
{
    if(*background_color == Qt::white)
        return 0;
    if(*background_color == Qt::cyan)
        return 1;
    if(*background_color == Qt::darkGray)
        return 2;
    if(*background_color == Qt::darkRed)
        return 3;
    if(*background_color == Qt::darkGreen)
        return 4;
    if(*background_color == Qt::darkBlue)
        return 5;
    if(*background_color == Qt::darkYellow)
        return 6;

    return 0;
}

void SimulationState::setColor(int code, PhObject& obj)
{
    switch (code)
    {
    case 0:
        obj.setColor(Qt::black);
        break;
    case 1:
        obj.setColor(Qt::red);
        break;
    case 2:
        obj.setColor(Qt::green);
        break;
    case 3:
        obj.setColor(Qt::blue);
        break;
    case 4:
        obj.setColor(Qt::yellow);
        break;

    default:
        obj.setColor(Qt::black);
        break;
    }
}

void SimulationState::setBackgroundColor(int code)
{
    switch (code)
    {
    case 0:
        *background_color = Qt::white;
        break;
    case 1:
        *background_color = Qt::blue;
        break;
    case 2:
        *background_color = Qt::darkGray;
        break;
    case 3:
        *background_color = Qt::darkRed;
        break;
    case 4:
        *background_color = Qt::darkGreen;
        break;
    case 5:
        *background_color = Qt::darkBlue;
        break;
    case 6:
        *background_color = Qt::darkYellow;
        break;

    default:
        *background_color = Qt::white;
        break;
    }
}

void SimulationState::setCollisionMode(int mode)
{
    switch (mode)
    {
    case 0:
        *collision_mode = CollisionMode::MERGE;
        break;
    case 1:
        *collision_mode = CollisionMode::ELASTIC;
        break;
    case 2:
        *collision_mode = CollisionMode::NOT_ELASTIC;
        break;

    default:
        *collision_mode = CollisionMode::MERGE;
        break;
    }
}

bool SimulationState::WriteDataInFile(QString way /*= ""*/)
{
    if(way == "")
        way = this->file_way;

    QFile WriteInFile("." + way);

    if(!WriteInFile.open(QIODevice::Text | QIODevice::WriteOnly))
        return false;
    else
    {
        QTextStream writer(&WriteInFile);
        writer.setRealNumberPrecision(20);

        writer << *G << endl;
        writer << *K << endl;
        writer << *collision_mode << endl;
        writer << getBackGroundColorCode() << endl;
        writer << CamX << endl;
        writer << CamY << endl;

        for(auto &var : *Objects)
        {
            writer << var.getName() << endl;;
            writer << (double)var.getMass() << endl;;
            writer << (double)var.getQ() << endl;;
            writer << (double)var.getRadius() << endl;;
            writer << (double)var.getXPosition() << endl;;
            writer << (double)var.getYPosition() << endl;;
            writer << (double)var.getXSpeed() << endl;;
            writer << (double)var.getYSpeed() << endl;;
            writer << getColorCode(var) << endl;;
            if(var.getStatic())
                writer << "1" << endl;
            else
                writer << "0" << endl;
        }

        WriteInFile.close();
    }

    return true;
}

bool SimulationState::ReadDataFromFile(QString way /*= ""*/)
{
    if(way == "")
        way = this->file_way;

    QFile WriteInFile(way);

    double ms, q, r, xp, yp, xs, ys;
    int cm, bc;
    int stat;

    QString name;


    if(!WriteInFile.open(QIODevice::Text | QIODevice::ReadOnly))
        return false;
    else
    {
        QTextStream writer(&WriteInFile);
        writer.setRealNumberPrecision(20);

        writer >> *G;
        writer >> *K;
        writer >> cm;
        writer >> bc;
        writer >> *CamX;
        writer >> *CamY;

        setCollisionMode(cm);
        setBackgroundColor(bc);

        for(auto &var : *Objects)
        {
            writer >> name;
            writer >> ms;
            writer >> q;
            writer >> r;
            writer >> xp;
            writer >> yp;
            writer >> xs;
            writer >> ys;
            writer >> bc;
            writer >> stat;

            var.setName(name);
            var.setMass(ms);
            var.setQ(q);
            var.setRadius(r);
            var.setXPosition(xp);
            var.setYPosition(yp);
            var.setXSpeed(xs);
            var.setYSpeed(ys);
            setColor(bc, var);
            var.setStatic(stat == 1);
        }

        WriteInFile.close();
    }

    return true;
}

void SimulationState::setFileWay(QString &way)
{
    this->file_way = way;
}

/*void SimulationState::setPCollisionMode(CollisionMode *p)
{
    collision_mode = p;
}

void SimulationState::setPG(double *p)
{
    G = p;
}

void SimulationState::setPK(double *p)
{
    K = p;
}

void SimulationState::sPCamX(int *p)
{
    CamX = p;
}

void SimulationState::sPCamY(int *p)
{
    CamY = p;
}

void SimulationState::setPColor(QColor *p)
{
   background_color = p;
}

void SimulationState::setPVector(QVector<PhObject> *p)
{
    Objects = p;
}*/

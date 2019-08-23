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

    QFile WriteInFile(way);

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
        writer << *CamX << endl;
        writer << *CamY << endl;
        writer << Objects->size() << endl;

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

int SimulationState::ReadDataFromFile(QString way /*= ""*/)
{
    if(way == "")
        way = this->file_way;

    QFile WriteInFile(way);

    double ms, q, r, xp, yp, xs, ys;
    double cx, cy;
    double G, K;
    int col;
    int cm, bc;
    int stat;
    int count, real_count = 0;

    QString name;

    bool succes = true;

    QString buffer;

    QRegExp float_reg("\\-?\\d{1,}\\.?\\d{1,}e?\\-?\\d{0,}");
    float_reg.setPatternSyntax(QRegExp::RegExp);

    QRegExp int_reg("\\-?\\d{1,}");
    int_reg.setPatternSyntax(QRegExp::RegExp);

    if(!WriteInFile.open(QIODevice::Text | QIODevice::ReadOnly))
        return 2;
    else
    {
        QTextStream reader(&WriteInFile);
        reader.setRealNumberPrecision(20);

        buffer = reader.readLine();
        G = buffer.toDouble();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        K = buffer.toDouble();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        cm = buffer.toInt();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        bc = buffer.toInt();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        cx = buffer.toDouble();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        cy = buffer.toDouble();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        buffer = reader.readLine();
        count = buffer.toInt();
        succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);


        if(count < 0)
            return 1;

        QVector<PhObject> Tmp_vector(count);

        if(succes)
        {
        for(auto &var : Tmp_vector)
        {
            real_count++;

            buffer = reader.readLine();
            name = buffer;

            buffer = reader.readLine();
            ms = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            q = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            r = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            xp = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            yp = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            xs = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            ys = buffer.toDouble();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            col = buffer.toInt();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            stat = buffer.toInt();
            succes &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            if(!succes)
                break;

            var.setName(name);
            var.setMass(ms);
            var.setQ(q);
            var.setRadius(r);
            var.setXPosition(xp);
            var.setYPosition(yp);
            var.setXSpeed(xs);
            var.setYSpeed(ys);
            setColor(col, var);
            var.setStatic(stat == 1);
        }
        }

        succes &= count == real_count;

        if(!succes)
            return 1;

        *(this->G) = G;
        *(this->K) = K;
        setCollisionMode(cm);
        setBackgroundColor(bc);
        *(this->CamX) = cx;
        *(this->CamY) = cy;

        *Objects = Tmp_vector;

        WriteInFile.close();
    }

    return 0;
}

void SimulationState::setFileWay(QString &way)
{
    this->file_way = way;
}

void SimulationState::setEmptyWay()
{
    file_way = "";
}


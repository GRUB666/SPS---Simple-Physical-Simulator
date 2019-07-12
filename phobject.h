#ifndef PHOBJECT_H
#define PHOBJECT_H

#include <QColor>
#include <QString>
#include <QVector>

//Стрктура шаблона генерации
struct GeneratePattern
{
    long double q1, q2, m1, m2; //Пределы для значений
    double rad1, rad2;

    int free_space; //Свободное пространство

    double start_speed;
    QVector<QColor> colors; //Цвета для генерации

    bool rad_auto; //Режим установки радиуса (пропорционально или произвольно)

    double p; //"Плотность"

    double max_speed;
    double tangentum_speed;
    QString name;

    int x0, y0; //Центры генерации

    GeneratePattern()
    {
        q1 = 0;
        q2 = 0;
        m1 = 1;
        m2 = 2;
        rad1 = 1;
        rad2 = 2;
        free_space = 11000;
        start_speed = 0;
        max_speed = 0;
        colors.push_back(Qt::black);
        rad_auto = true;
        p = 1;
        name = "Object";
        x0 = 0;
        y0 = 0;
    }
};

//Класс Physical Object
class PhObject
{
private:
    long double x, y;

    long double xSpeed, ySpeed;
    long double xAccel, yAccel;

    long double mass;
    long double radius;
    long double q;

    QColor color;

    QString Name;

    bool isStatic;

    bool inFocus; //В фокусе ли сейчас объект (необходимо ли подсвечивать)

public:
    PhObject(long double mass = 1, long double rad = 1, long double q = 0, long double x = 0, long double y = 0, long double xS = 0, long double yS = 0);

    //----Setters----
    void setPosition (long double x, long double y); //Position methods
    void setXPosition(long double x          );
    void setYPosition(long double y          );

    void setSpeed (long double xS, long double yS); //Speed methods
    void setXSpeed(long double xS           );
    void setYSpeed(long double yS           );

    void setMass   (long double mass  );
    void setRadius (long double radius);
    void setQ      (long double q);
    void setColor  (QColor color );
    void setName   (QString name);
    void setStatic (bool val);
    void setFocus  (bool val);
    void setXAccel (long double xA) { this->xAccel = xA; }
    void setYAccel (long double yA) { this->yAccel = yA; }
    void setAccel  (long double xA, long double yA) { this->xAccel = xA; this->yAccel = yA; }


    //----Getters----
    long double getXPosition();
    long double getYPosition();

    long double getXSpeed();
    long double getYSpeed();

    long double getXAccel() { return xAccel; }
    long double getYAccel() { return yAccel; }

    long double  getMass();
    long double  getRadius();
    long double  getQ();
    QColor  getColor();
    QString getName();
    bool getStatic();
    bool getFocus();


    bool operator != (PhObject& par)
    {
      if(this->getXPosition() != par.getXPosition())
          return true;
      if(this->getYPosition() != par.getYPosition())
          return true;
      if(this->getXSpeed() != par.getXSpeed())
          return true;
      if(this->getYSpeed() != par.getYSpeed())
          return true;
      if(this->getMass() != par.getMass())
          return true;
      if(this->getRadius() != par.getRadius())
          return true;
      if(this->getQ() != par.getQ())
          return true;
      if(this->getColor() != par.getColor())
          return true;
      if(this->getName() != par.getName())
          return true;
      if(this->getStatic() != par.getStatic())
          return true;

      return false;
    }

   bool operator == (PhObject& par)
   {
       return !(*this != par);
   }
};

#endif // PHOBJECT_H

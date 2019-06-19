#ifndef PHOBJECT_H
#define PHOBJECT_H

#include <QColor>
#include <QString>
#include <QVector>

struct GeneratePattern
{
    long double q1, q2, m1, m2;
    double rad1, rad2;
    int free_space;
    double start_speed;
    QVector<QColor> colors;
    bool rad_auto;
    double p;
    double max_speed;
    double tangentum_speed;
    QString name;
    int x0, y0;

    GeneratePattern()
    {
        q1 = 0;
        q2 = 0;
        m1 = 1;
        m2 = 2;
        rad1 = 1;
        rad2 = 2;
        free_space = 9000;
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

class PhObject
{
private:
    long double x, y;
    long double xSpeed, ySpeed;
    long double mass;
    long double radius;
    long double q;
    QColor color;
    QString Name;
    bool isStatic;
    bool inFocus;

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


    //----Getters----
    long double getXPosition();
    long double getYPosition();

    long double getXSpeed();
    long double getYSpeed();

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
          return false;
      if(this->getYPosition() != par.getYPosition())
          return false;
      if(this->getXSpeed() != par.getXSpeed())
          return false;
      if(this->getYSpeed() != par.getYSpeed())
          return false;
      if(this->getMass() != par.getMass())
          return false;
      if(this->getRadius() != par.getRadius())
          return false;
      if(this->getQ() != par.getQ())
          return false;
      if(this->getColor() != par.getColor())
          return false;
      if(this->getName() != par.getName())
          return false;

      return true;
    }

   bool operator == (PhObject& par)
   {
       return !(*this != par);
   }
};

#endif // PHOBJECT_H

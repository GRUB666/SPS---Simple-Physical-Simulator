#ifndef PHOBJECT_H
#define PHOBJECT_H

#include <QColor>
#include <QString>

class PhObject
{
private:
    double x, y;
    double xSpeed, ySpeed;
    double mass;
    double radius;
    double q;
    QColor color;
    QString Name;
    bool isStatic;
    bool inFocus;

public:
    PhObject(double mass = 0, double rad = 10, double q = 0, double x = 0, double y = 0, double xS = 0, double yS = 0);

    //----Setters----
    void setPosition (double x, double y); //Position methods
    void setXPosition(double x          );
    void setYPosition(double y          );

    void setSpeed (double xS, double yS); //Speed methods
    void setXSpeed(double xS           );
    void setYSpeed(double yS           );

    void setMass   (double mass  );
    void setRadius (double radius);
    void setQ      (double q);
    void setColor  (QColor color );
    void setName   (QString name);
    void setStatic (bool val);
    void setFocus  (bool val);


    //----Getters----
    double getXPosition();
    double getYPosition();

    double getXSpeed();
    double getYSpeed();

    double  getMass();
    double  getRadius();
    double  getQ();
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

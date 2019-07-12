#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>

enum CollisionMode {MERGE, ELASTIC, NOT_ELASTIC};


//Структура настроек
struct Settings
{

public:
    bool SET_PAUSE_AFTER_CREATE;
    bool SET_PAUSE_AFTER_DELETE;
    bool SET_PAUSE_AFTER_RESTART;
    bool CAMERA_BUFFER_ENABLE;
    CollisionMode COLLISION_MODE;
    double G, K;
    double SCALE_SPEED;
    QColor BACKGROUND_COLOR;
    bool RENDER_MODE; //0 - Без deltaTime, 1 - С deltaTime
    double SIMULATION_SPEED;
    bool OPEN_FULLSCREEN;

public:
    Settings();

    Settings(bool SPAC, bool SPAD, bool SPAR, bool CBE, CollisionMode CM, double G,
             double K, double MS, QColor BC, bool RM, double SS, bool OF);

    void SetDefaultSettings();
    void setBackgroundColor(int code);
    void setCollisionMode(int mode);

    bool operator == (Settings& obj);
    bool operator != (Settings& obj);

};

#endif // SETTINGS_H

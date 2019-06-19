#include "settings.h"

Settings::Settings()
{

}

Settings::Settings(bool SPAC, bool SPAD, bool SPAR, bool CBE, CollisionMode CM, double G, double K, double MS, QColor BC, bool RM, double SS, bool OF)
{
    SET_PAUSE_AFTER_CREATE = SPAC;
    SET_PAUSE_AFTER_DELETE = SPAD;
    SET_PAUSE_AFTER_RESTART = SPAR;
    CAMERA_BUFFER_ENABLE = CBE;
    COLLISION_MODE = CM;
    this->G = G;
    this->K = K;
    SCALE_SPEED = MS;
    BACKGROUND_COLOR = BC;
    RENDER_MODE = RM;
    SIMULATION_SPEED = SS;
    OPEN_FULLSCREEN = OF;
}

void Settings::SetDefaultSettings()
{
    SET_PAUSE_AFTER_CREATE = true;
    SET_PAUSE_AFTER_DELETE = false;
    SET_PAUSE_AFTER_RESTART = true;
    CAMERA_BUFFER_ENABLE = true;
    COLLISION_MODE = CollisionMode::MERGE;
    this->G = 1;
    this->K = 1;
    SCALE_SPEED = 0.05;
    BACKGROUND_COLOR = Qt::white;
    RENDER_MODE = false;
    SIMULATION_SPEED = 0.01;
    OPEN_FULLSCREEN = false;
}

void Settings::setBackgroundColor(int code)
{
    switch (code)
    {
    case 0:
        BACKGROUND_COLOR = Qt::white;
        break;
    case 1:
        BACKGROUND_COLOR = Qt::cyan;
        break;
    case 2:
        BACKGROUND_COLOR = Qt::darkGray;
        break;
    case 3:
        BACKGROUND_COLOR = Qt::darkRed;
        break;
    case 4:
        BACKGROUND_COLOR = Qt::darkGreen;
        break;
    case 5:
        BACKGROUND_COLOR = Qt::darkBlue;
        break;
    case 6:
        BACKGROUND_COLOR = Qt::darkYellow;
        break;

    default:
        BACKGROUND_COLOR = Qt::white;
        break;
    }
}

void Settings::setCollisionMode(int mode)
{
    switch (mode)
    {
    case 0:
        COLLISION_MODE = CollisionMode::MERGE;
        break;
    case 1:
        COLLISION_MODE = CollisionMode::ELASTIC;
        break;
    case 2:
        COLLISION_MODE = CollisionMode::NOT_ELASTIC;
        break;

    default:
        COLLISION_MODE = CollisionMode::MERGE;
        break;
    }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QListWidgetItem>
#include <QComboBox>
#include <QPalette>
#include <QMessageBox>
#include <cmath>
#include <QKeyEvent>
#include <QShortcut>
#include <ctime>
#include <chrono>
#include <thread>
#include <random>
#include <QDir>
#include <QCoreApplication>
#include "phobject.h"
#include "generatewidget.h"
#include "settingswidget.h"
#include "simulationstate.h"
#include "settings.h"



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QVector<PhObject> Objects;
    QVector<PhObject> Buffer_Objects;
    QVector<PhObject> Save_Buffer_Objects;
    QVector<PhObject> Patterns;
    int current_index;
    int current_pattern;
    bool pattern_mode;

    QString settings_way;

    QTimer* timer;

    QShortcut *keyAdd;
    QShortcut *keyDelete;
    QShortcut *keyCenter;
    QShortcut *keyScale;
    QShortcut *keyCheckPoint;
    QShortcut *keyRestart;
    QShortcut *keyPause;
    QShortcut *keySetCenter;
    QShortcut *keyGen;
    QShortcut *keyFollow;

    double G, k;
    bool isPause;
    bool FullScreenMode;
    double sim_speed;
    int delta;
    bool follow_to_focus_object;
    CollisionMode Current_Collision_Mode;
    QColor Current_Background_Color;

    int BCamX, BCamY; //Буферы положений камеры

    //Настройки
private:
    SimulationState Simulation_State;
    Settings Programm_Settings;


protected:
    virtual void keyPressEvent(QKeyEvent* pe);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void setPause (bool val             );
    void setFocus (int index = -1);
    void printToPanel(PhObject&, bool sp = false);
    void setColorBox(PhObject &toSet);
    void clearPrintPanel();
    void updateList();
    void dropFocus();
    QColor getColorBox();
    int getPowerMnog(QComboBox*);
    void updateViewport();
    void setConstFields();
    void setFullScreenMode(bool val);
    bool checkIndexValid(int index, QVector<PhObject> &vec);
    long double getDistance(int obj1, int obj2);
    long double getDistance(double px, double py, PhObject& obj);
    void followToObject(PhObject& obj);
    void clearObjectsVector();
    void loadSettings();
    void saveSettings();
    void newModel();
    bool hasModifided();

private:
    Ui::MainWindow *ui;

private slots:
   void ForceCalc();
   void setPause ();
   void Restart  ();
   void addObject();
   void deleteObject(int index = -1);
   void changeParameters();
   void on_name_line_textEdited(const QString &arg1);
   void on_ListObjects_currentRowChanged(int currentRow);
   void addPattern();
   void deletePattern();
   void updatePatternsList();
   void setPatternMode(bool val);
   void on_Patterns_list_currentRowChanged(int currentRow);
   void on_Patterns_list_clicked(const QModelIndex &index);
   void on_ListObjects_clicked(const QModelIndex &index);
   void changeScaleSlot(int value);
   void changeCamLabel(int x, int y);
   void moveCameraToCenter();
   void dropCameraScale();
   void setBuffer();
   void on_g_line_textEdited(const QString &arg1);
   void on_k_line_textEdited(const QString &arg1);
   void on_ListObjects_doubleClicked(const QModelIndex &index);
   void changeFollowSetting();
   void changeFollowSetting(bool val);
   void makeCenter();
   void makeNullSpeed();
   void OpenGenerateWidget();
   void randomGenerate(GeneratePattern& pattern, int count);
   void changeFullScreenMode();
   void clearAllObjectsSlot();
   void OpenSettings();
   void createNewModel();
   //void saveModel();
   //void openModel();
};

#endif // MAINWINDOW_H

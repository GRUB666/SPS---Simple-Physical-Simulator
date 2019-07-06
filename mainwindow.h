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
#include <QFileDialog>
#include <QCloseEvent>
#include <QMouseEvent>

#include "phobject.h"
#include "generatewidget.h"
#include "settingswidget.h"
#include "simulationstate.h"
#include "settings.h"
#include "documentationwidget.h"
#include "hellowindow.h"

#define PI 3.141592653589793238462643



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void mousePressEvent(QMouseEvent* );

private:
    QVector<PhObject> Objects;
    QVector<PhObject> Buffer_Objects;
    QVector<PhObject> Save_Buffer_Objects;
    QVector<PhObject> Patterns;
    int current_index;
    int current_pattern;
    bool pattern_mode;

    QString settings_way;
    QString patterns_way;

    QString version;

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
    int delta;
    bool follow_to_focus_object;
    CollisionMode Current_Collision_Mode;
    QColor Current_Background_Color;

    double BCamX, BCamY; //Буферы положений камеры

    //Настройки
private:
    SimulationState Simulation_State;
    Settings Programm_Settings;


protected:
    virtual void keyPressEvent(QKeyEvent* pe);

public:
    explicit MainWindow(QString version, QWidget *parent = nullptr);
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
    bool vectorEquals(QVector<PhObject> &vec1, QVector<PhObject> &vec2);
    void save();
    void open(QString prev_way = "");
    void setNewWindowTitle(QString& way);
    void savePatterns();
    void loadPatterns();
    void addStandartPattern();
    long double getDistanceOfCollision(int i, int j);
    void ForceCalc();
    void MoveObjects();
    void CollisionsCheck();

private:
    Ui::MainWindow *ui;

private slots:
   void everyTickSlot();
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
   void setPatternFocus(int index);
   void on_Patterns_list_clicked(const QModelIndex &index);
   void on_ListObjects_clicked(const QModelIndex &index);
   void changeScaleSlot(double value);
   void changeScaleSlot(int value);
   void updateCameraLabel();
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
   void saveModel();
   void saveModelAs();
   void openModel();
   void openDocumentation(QString str = "Основная информация о программе");
   void openHello();

signals:
   void clickInArea();
};

#endif // MAINWINDOW_H

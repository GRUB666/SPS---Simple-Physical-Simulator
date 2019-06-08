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
#include "phobject.h"
#include "generatewidget.h"


namespace Ui {
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QVector<PhObject> Objects;
    QVector<PhObject> Buffer_Objects;
    QVector<PhObject> Patterns;
    int current_index;
    int current_pattern;
    bool pattern_mode;

    QTimer* timer;

    QShortcut *keyAdd;
    QShortcut *keyDelete;
    QShortcut *keyCenter;
    QShortcut *keyScale;
    QShortcut *keyCheckPoint;
    QShortcut *keyRestart;
    QShortcut *keyPause;
    QShortcut *keySetCenter;

    double G, k;
    bool isPause;
    bool FullScreenMode;
    double sim_speed;
    int delta;

public:
    bool SET_PAUSE_AFTER_CREATE;
    bool SET_PAUSE_AFTER_RESTART;
    bool FOLLOW_TO_FOCUS_OBJECT;

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
    void followToObject(PhObject& obj);



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
   void OpenGenerateWidget();
   void randomGenerate(GeneratePattern& pattern, int count);
};

#endif // MAINWINDOW_H

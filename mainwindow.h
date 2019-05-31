#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>

#include "phobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QVector<PhObject> Objects;
    QVector<PhObject> Buffer_Objects;
    PhObject *current_object;

    QTimer* timer;

    double G, k;
    bool isPause;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void setPause(bool val);
    void setFocus(PhObject* p = nullptr);

private:
    Ui::MainWindow *ui;

private slots:
   void ForceCalc();
   void setPause();
   void Restart();

};

#endif // MAINWINDOW_H

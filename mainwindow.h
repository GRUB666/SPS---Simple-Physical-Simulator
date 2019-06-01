#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QListWidgetItem>

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

    bool SET_PAUSE_AFTER_CREATE;
    bool SET_PAUSE_AFTER_RESTART;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void setPause (bool val             );
    void setFocus (int index, PhObject* p = nullptr);
    void printToPanel(PhObject&);
    void setColorBox(PhObject &toSet);
    void clearPrintPanel();
    void updateList();
    void dropFocus();
    QColor getColorBox();

private:
    Ui::MainWindow *ui;

private slots:
   void ForceCalc();
   void setPause ();
   void Restart  ();
   void addObject();
   void changeParameters();
   void on_name_line_textEdited(const QString &arg1);
   void on_ListObjects_currentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H

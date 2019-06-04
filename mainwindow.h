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
    QVector<PhObject> Patterns;
    int current_index;
    int current_pattern;
    bool pattern_mode;

    QTimer* timer;

    QPalette commonPalette;
    QPalette patternPalette;

    double G, k;
    bool isPause;

    bool SET_PAUSE_AFTER_CREATE;
    bool SET_PAUSE_AFTER_RESTART;
    bool FOLLOW_TO_FOCUS_OBJECT;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void setPause (bool val             );
    void setFocus (int index = -1);
    void printToPanel(PhObject&);
    void setColorBox(PhObject &toSet);
    void clearPrintPanel();
    void updateList();
    void dropFocus();
    QColor getColorBox();
    int getPowerMnog(QComboBox*);
    void updateViewport();

private:
    Ui::MainWindow *ui;

private slots:
   void ForceCalc();
   void setPause ();
   void Restart  ();
   void addObject();
   void deleteObject();
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
};

#endif // MAINWINDOW_H

#ifndef HELLOWINDOW_H
#define HELLOWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QMouseEvent>
#include <QEvent>
#include <QMainWindow>

#include "documentationwidget.h"

namespace Ui {
class HelloWindow;
}

class HelloWindow : public QDialog
{
    Q_OBJECT


public:
    explicit HelloWindow(QString version, QWidget *parent = nullptr);
    ~HelloWindow();

private:
    Ui::HelloWindow *ui;

private slots:
    void focusSlot();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // HELLOWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->showMessage("Приложение загружено");              //Первоначальная инициализация приложения
    ui->add_button->setIcon(QIcon(QPixmap(":/img/img/plus.png")));
    ui->delete_button->setIcon(QIcon(QPixmap("://img/krest.png")));
    ui->delete_button->setEnabled(false);
    ui->restart_button->setIcon(QIcon(QPixmap("://img/restart.png")));
    timer = new QTimer;  //Таймер
    timer->setInterval(0);
    setPause(true);
    setFocus();

    connect(timer, SIGNAL(timeout()), this, SLOT(ForceCalc()));            //Соединение сигналов и слотов
    connect(ui->pause_button, SIGNAL(clicked()), this, SLOT(setPause()));
    connect(ui->restart_button, SIGNAL(clicked()), this, SLOT(Restart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ForceCalc()
{
    //Расчёт сил, редактирование вектора
    //Перерисовка
}

void MainWindow::setPause()
{
    setPause(!isPause);
}

void MainWindow::Restart()
{
    if(Objects != Buffer_Objects)
    {
        Objects = Buffer_Objects;
    }

    setPause(true);
}

void MainWindow::setPause(bool val)
{
    isPause = val;

    if(isPause)
    {
        ui->viewport->setStyleSheet("#viewport { border: 5px solid red; }");
        ui->pause_button->setIcon(QIcon(QPixmap("://img/start.png")));
        timer->stop();
    }

    else
    {
        ui->viewport->setStyleSheet("#viewport { border: 0px solid black; }");
        ui->pause_button->setIcon(QIcon(QPixmap("://img/stop.png")));
        timer->start(0);
    }
}

void MainWindow::setFocus(PhObject *p)
{
    current_object = p;

    if(p == nullptr)
    {
        ui->Propertites_box->setEnabled(false);
    }

    else
    {
        ui->Propertites_box->setEnabled(true);
        //Тут ещё куча пожилого кода
    }
}

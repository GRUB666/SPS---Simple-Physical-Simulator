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
    setFocus(0);

    QRegExp valide_reg("\\-?\\d{1,}\\.?\\d{1,}");
    QValidator *validator = new QRegExpValidator(valide_reg, this);

    ui->mass_line->setValidator(validator);
    ui->q_line->setValidator(validator);
    ui->x_line->setValidator(validator);
    ui->y_line->setValidator(validator);
    ui->xs_line->setValidator(validator);
    ui->ys_line->setValidator(validator);

    //----Установка настроек

    SET_PAUSE_AFTER_CREATE = true;
    SET_PAUSE_AFTER_RESTART = true;

    //----------------------

    connect(timer, SIGNAL(timeout()), this, SLOT(ForceCalc()));            //Соединение сигналов и слотов
    connect(ui->pause_button, SIGNAL(clicked()), this, SLOT(setPause()));
    connect(ui->restart_button, SIGNAL(clicked()), this, SLOT(Restart()));
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addObject()));

    connect(ui->mass_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->q_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->x_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->y_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->xs_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->ys_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->isMoveBox, SIGNAL(stateChanged(int)), this, SLOT(changeParameters()));
    connect(ui->color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
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
    Objects = Buffer_Objects;
    ui->viewport->setPaintVector(Objects);
    setFocus(0, nullptr);
    updateList();
    ui->viewport->update();

    if(SET_PAUSE_AFTER_RESTART)
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

void MainWindow::setFocus(int index, PhObject *p)
{
    current_object = p;

    if(p == nullptr)
    {
        ui->Propertites_box->setEnabled(false);
        clearPrintPanel();
    }

    else
    {
        dropFocus();
        p->setFocus(true);
        ui->ListObjects->setCurrentRow(index);
        ui->Propertites_box->setEnabled(true);
        printToPanel(*p);
    }
}



void MainWindow::addObject()
{
    PhObject obj;
    obj.setPosition(ui->viewport->getCamX(), ui->viewport->getCamY());
    Objects.push_back(obj);
    updateList();
    setFocus(Objects.size() - 1, &Objects[Objects.size() - 1]);
    ui->viewport->setPaintVector(Objects);
    ui->viewport->update();


    if(SET_PAUSE_AFTER_CREATE)
        setPause(true);
}


void MainWindow::printToPanel(PhObject &toPrint)
{
    ui->name_line->setText(toPrint.getName());
    ui->mass_line->setText(QString::number(toPrint.getMass()));
    ui->q_line->setText(QString::number(toPrint.getQ()));
    ui->x_line->setText(QString::number(toPrint.getXPosition()));
    ui->y_line->setText(QString::number(toPrint.getYPosition()));
    ui->xs_line->setText(QString::number(toPrint.getXSpeed()));
    ui->ys_line->setText(QString::number(toPrint.getYSpeed()));
    ui->isMoveBox->setChecked(toPrint.getStatic());

    setColorBox(toPrint);
}

void MainWindow::setColorBox(PhObject &toSet)
{
    if(toSet.getColor() == Qt::black)
        ui->color_box->setCurrentIndex(0);
    if(toSet.getColor() == Qt::red)
        ui->color_box->setCurrentIndex(1);
    if(toSet.getColor() == Qt::green)
        ui->color_box->setCurrentIndex(2);
    if(toSet.getColor() == Qt::blue)
        ui->color_box->setCurrentIndex(3);
    if(toSet.getColor() == Qt::yellow)
        ui->color_box->setCurrentIndex(4);
}

void MainWindow::clearPrintPanel()
{
    ui->name_line->setText("");
    ui->mass_line->setText("");
    ui->q_line->setText("");
    ui->x_line->setText("");
    ui->y_line->setText("");
    ui->xs_line->setText("");
    ui->ys_line->setText("");

    ui->color_box->blockSignals(true);
    ui->color_box->setCurrentIndex(0);
    ui->color_box->blockSignals(false);

    ui->isMoveBox->blockSignals(true);
    ui->isMoveBox->setChecked(false);
    ui->isMoveBox->blockSignals(false);
}

void MainWindow::updateList()
{
    ui->ListObjects->blockSignals(true);
    ui->ListObjects->clear();
    ui->ListObjects->blockSignals(false);

    for(auto var : Objects)
    {
        ui->ListObjects->addItem(var.getName());
    }
}

void MainWindow::dropFocus()
{
    for(auto var : Objects)
    {
        var.setFocus(false);
    }
}

QColor MainWindow::getColorBox()
{
    if(ui->color_box->currentIndex() == 0)
        return Qt::black;
    if(ui->color_box->currentIndex() == 1)
        return Qt::red;
    if(ui->color_box->currentIndex() == 2)
        return Qt::green;
    if(ui->color_box->currentIndex() == 3)
        return Qt::blue;
    if(ui->color_box->currentIndex() == 4)
        return Qt::yellow;

    return Qt::black;
}


void MainWindow::changeParameters()
{
    current_object->setName(ui->name_line->text());
    current_object->setMass(ui->mass_line->text().toDouble());
    current_object->setQ(ui->q_line->text().toDouble());
    current_object->setPosition(ui->x_line->text().toDouble(), ui->y_line->text().toDouble());
    current_object->setSpeed(ui->xs_line->text().toDouble(), ui->ys_line->text().toDouble());
    current_object->setStatic(ui->isMoveBox->isChecked());
    current_object->setColor(getColorBox());

    ui->viewport->setPaintVector(Objects);

    ui->viewport->update();
}


void MainWindow::on_name_line_textEdited(const QString &arg1)
{
    changeParameters();
    updateList();
}

void MainWindow::on_ListObjects_currentRowChanged(int currentRow)
{
    setFocus(0, &Objects[currentRow]);
}

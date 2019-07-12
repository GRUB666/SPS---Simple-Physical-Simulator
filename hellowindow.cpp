#include "hellowindow.h"
#include "ui_hellowindow.h"

#include <QTimer>


HelloWindow::HelloWindow(QString version, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::HelloWindow)
{
    ui->setupUi(this);

    ui->label_2->setText(ui->label_2->text() + " " + version);

    this->setWindowFlag(Qt::ToolTip);

    ui->pushButton->setFocus();
}

HelloWindow::~HelloWindow()
{
    delete ui;
}

void HelloWindow::on_pushButton_clicked()
{
    close();
}

void HelloWindow::on_pushButton_2_clicked()
{
    DocumentationWidget wdg;

    wdg.setWindowIcon(windowIcon());

    close();

    wdg.exec();
}

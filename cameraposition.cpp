#include "cameraposition.h"
#include "ui_cameraposition.h"

#include <QRegExpValidator>
#include "documentationwidget.h"

CameraPosition::CameraPosition(double* cPx, double* cPy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraPosition)
{
    ui->setupUi(this);

    cx = cPx;
    cy = cPy;

    ui->x_line->setText(QString::number(int(*cx)));
    ui->y_line->setText(QString::number(int(*cy)));

    QRegExp rad("\\-?\\d{1,}");
    QValidator *rad_val = new QRegExpValidator(rad, this);

    ui->x_line->setValidator(rad_val);
    ui->y_line->setValidator(rad_val);
}

CameraPosition::~CameraPosition()
{
    delete ui;
}

void CameraPosition::on_x_line_textEdited(const QString &arg1)
{

}

void CameraPosition::on_pushButton_clicked()
{
    *cx = ui->x_line->text().toInt();
    *cy = ui->y_line->text().toInt();

    close();
}

void CameraPosition::on_pushButton_2_clicked()
{
    close();
}

void CameraPosition::on_label_linkActivated(const QString &link)
{
    DocumentationWidget* win = new DocumentationWidget("Камера");
    win->setWindowIcon(windowIcon());
    win->exec();
}

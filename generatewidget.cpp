#include "generatewidget.h"
#include "ui_generatewidget.h"
#include <QDebug>
#include <QValidator>
#include <QMessageBox>

GenerateWidget::GenerateWidget(QWidget *parent, int CamX, int CamY, bool* succes, GeneratePattern* pattern, int* count) :
    QDialog(parent),
    ui(new Ui::GenerateWidget)
{
    ui->setupUi(this);

    ui->free_lab->setToolTip("Средняя площадь для одного объекта");

    QRegExp massq("\\-?\\d{1,}");
    QRegExp pfrsp("\\d{1,}\\.?\\d{1,}");
    QRegExp rad("\\d{1,}");

    this->succes = succes;
    this->pattern = pattern;
    *(this->succes) = false;
    this->count = count;
    *(this->count) = 0;

    this->CamX = CamX;
    this->CamY = CamY;
    setRadiusMode(true);

    QValidator *massq_val = new QRegExpValidator(massq, this);
    QValidator *pfrsp_val = new QRegExpValidator(pfrsp, this);
    QValidator *rad_val = new QRegExpValidator(rad, this);

    ui->mas1->setValidator(massq_val);
    ui->mass2->setValidator(massq_val);
    ui->q1->setValidator(massq_val);
    ui->q2->setValidator(massq_val);

    ui->p_line->setValidator(pfrsp_val);
    ui->rad1->setValidator(rad_val);
    ui->rad2->setValidator(rad_val);
    ui->count->setValidator(rad_val);

    ui->space->setValidator(pfrsp_val);
    ui->speed->setValidator(pfrsp_val);

    count = 0;
}



GenerateWidget::~GenerateWidget()
{
    delete ui;
}

void GenerateWidget::setRadiusMode(bool val)
{
    pattern->rad_auto = val;

    ui->rad_lab->setEnabled(!val);
    ui->rad1_lab->setEnabled(!val);
    ui->rad2_lab->setEnabled(!val);
    ui->rad1->setEnabled(!val);
    ui->rad2->setEnabled(!val);

    ui->p_lab->setEnabled(val);
    ui->p_line->setEnabled(val);
    ui->ed_lab->setEnabled(val);

    on_rad1_textEdited(ui->rad1->text());
    on_rad2_textEdited(ui->rad2->text());
}

void GenerateWidget::on_radprop_clicked()
{
    setRadiusMode(ui->radprop->isChecked());
}

void GenerateWidget::on_radnprop_clicked()
{
    setRadiusMode(ui->radprop->isChecked());
}

void GenerateWidget::on_lineEdit_textEdited(const QString &arg1)
{
    pattern->name = arg1;
}

void GenerateWidget::on_mas1_textEdited(const QString &arg1)
{
    pattern->m1 = arg1.toInt();
}

void GenerateWidget::on_mass2_textEdited(const QString &arg1)
{
    pattern->m2 = arg1.toInt();
}

void GenerateWidget::on_q1_textEdited(const QString &arg1)
{
    pattern->q1 = arg1.toInt();
}

void GenerateWidget::on_q2_textEdited(const QString &arg1)
{
    pattern->q2 = arg1.toInt();
}

void GenerateWidget::on_p_line_textEdited(const QString &arg1)
{
    pattern->p = arg1.toDouble();
}

void GenerateWidget::on_rad1_textEdited(const QString &arg1)
{
    pattern->rad1 = arg1.toInt();
}

void GenerateWidget::on_rad2_textEdited(const QString &arg1)
{
    pattern->rad2 = arg1.toInt();
}

void GenerateWidget::on_space_textEdited(const QString &arg1)
{
    pattern->free_space = arg1.toDouble();
}

void GenerateWidget::on_speed_textEdited(const QString &arg1)
{
    pattern->start_speed = arg1.toDouble();
}


void GenerateWidget::on_cam1_clicked()
{
    pattern->x0 = CamX;
    pattern->y0 = CamY;
}

void GenerateWidget::on_coord_clicked()
{
    pattern->x0 = 0;
    pattern->y0 = 0;
}

void GenerateWidget::setColor()
{
   if(!ui->black->isChecked() && !ui->red->isChecked() && !ui->green->isChecked() && !ui->blue->isChecked() && !ui->yellow->isChecked())
   {
       ui->black->setChecked(true);
   }

   pattern->colors.clear();

   if(ui->black->isChecked())
       pattern->colors.push_back(Qt::black);
   if(ui->red->isChecked())
       pattern->colors.push_back(Qt::red);
   if(ui->green->isChecked())
       pattern->colors.push_back(Qt::green);
   if(ui->blue->isChecked())
       pattern->colors.push_back(Qt::blue);
   if(ui->yellow->isChecked())
       pattern->colors.push_back(Qt::yellow);
}

void GenerateWidget::on_pushButton_clicked()
{
    bool suc = true;

    if(pattern->m1 == 0 || pattern->m2 == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Значение массы не может быть равным нулю!");
        suc = false;
    }

    if((pattern->rad1 <= 0 || pattern->rad2 <= 0) && ui->radnprop->isChecked())
    {
        QMessageBox::warning(this, "Предупреждение", "Значение радиуса не может быть равным нулю или отрицательным!");
        suc = false;
    }

    if(count == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Количество объектов не может равняться нулю!");
        suc = false;
    }

    if(*count > 500)
    {
        QMessageBox::warning(this, "Предупреждение", "При таком количестве объектов [" + QString::number(*count) + "] работа программы может быть нестабильна");
    }

    if(pattern->m1 > pattern->m2)
    {
        QMessageBox::warning(this, "Предупреждение", "Верхний предел массы не может быть меньше нижнего!");
        suc = false;
    }

    if(pattern->q1 > pattern->q2)
    {
        QMessageBox::warning(this, "Предупреждение", "Верхний предел заряда не может быть меньше нижнего!");
        suc = false;
    }

    if(pattern->rad1 > pattern->rad2)
    {
        QMessageBox::warning(this, "Предупреждение", "Верхний предел радиуса не может быть меньше нижнего!");
        suc = false;
    }

     *(this->succes) = suc;

    if(suc)
    {
       close();
    }

}

void GenerateWidget::on_count_textEdited(const QString &arg1)
{
    *count = arg1.toInt();
}

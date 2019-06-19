#include "generatewidget.h"
#include "ui_generatewidget.h"
#include <QDebug>
#include <QValidator>
#include <QMessageBox>

GenerateWidget::GenerateWidget(QVector<PhObject> pat_list, QWidget *parent, bool* succes, GeneratePattern* Sppattern, int* count) :
    QDialog(parent),
    ui(new Ui::GenerateWidget)
{
    ui->setupUi(this);

    ui->free_lab->setToolTip("Средняя площадь для одного объекта");

    QRegExp massq("\\-?\\d{1,}\\.?\\d{1,}");
    QRegExp pfrsp("\\d{1,}\\.?\\d{1,}");
    QRegExp rad("\\d{1,}");

    this->succes = succes;
    this->pattern = Sppattern;
    *(this->succes) = false;
    this->count = count;
    *(this->count) = 0;
    this->pat_list = pat_list;

    this->CamX = Sppattern->x0;
    this->CamY = Sppattern->y0;

    QValidator *massq_val = new QRegExpValidator(massq, this);
    QValidator *pfrsp_val = new QRegExpValidator(pfrsp, this);
    QValidator *rad_val = new QRegExpValidator(rad, this);

    ui->mas1->setValidator(massq_val);
    ui->mass2->setValidator(massq_val);
    ui->q1->setValidator(massq_val);
    ui->q2->setValidator(massq_val);

    ui->p_line->setValidator(pfrsp_val);

    ui->rad1->setValidator(pfrsp_val);
    ui->rad2->setValidator(pfrsp_val);
    ui->count->setValidator(rad_val);

    ui->free_lab->setToolTip("Пространство, которое в среднем выделяется под один объект (Определяет среднее растояние до других объектов)");
    ui->max_speed_lab->setToolTip("Максимальное значение случайной начальной скорости в случайном направлении");
    ui->rad_speed_lab->setToolTip("Начальная скорость, с которой объект отдаляется от центра (Эффект большого взрыва)");
    ui->tang_speed_lab->setToolTip("Начальная скорость, с которой объект движется вокруг центра (Эффект вихря)");
    ui->center_lab->setToolTip("Точка, вокруг которой будет происходить генерация");
    ui->base_name_lab->setToolTip("Базовое имя, с которым будут создаваться новые объекты");
    ui->mass_lab->setToolTip("Введите в поле [От] минимальное значение массы объекта, а в поле [До] максимальное значение (Масса не может быть равна нулю!)");
    ui->q_lab->setToolTip("Введите в поле [От] минимальное значение заряда объекта, а в поле [До] максимальное значение");
    ui->radprop->setToolTip("Значение радиуса будет зависеть от массы, с коэфициентом пропорциональности, указанным ниже (MASS x P)");
    ui->radnprop->setToolTip("Значение радиуса будет генерироваться случайно в обозначенных ниже пределах (Видимый размер объекта не будет коррелировать с его массой)");
    ui->rad_lab->setToolTip("Введите в поле [От] минимальное значение радиуса объекта, а в поле [До] максимальное значение (Радиус должен быть больше нуля!)");

    ui->space->setValidator(pfrsp_val);
    ui->mx_speed_line->setValidator(pfrsp_val);
    ui->speed->setValidator(pfrsp_val);
    ui->tangentum_speed->setValidator(pfrsp_val);

    firstOut = true;

    fillPatternsList();
    FillFields();

    connect(ui->black, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->red, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->green, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->blue, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->yellow, SIGNAL(clicked()), this, SLOT(setColor()));

    ui->cam1->setText("Позиция камеры [" + QString::number(CamX) + "; " + QString::number(CamY) + "]");

   // resize(minimumSize());
}



GenerateWidget::~GenerateWidget()
{
    delete ui;
}

void GenerateWidget::FillFields()
{
    setRadiusMode(pattern->rad_auto);

    ui->lineEdit->setText(pattern->name);
    ui->radprop->setChecked(pattern->rad_auto);
    ui->radnprop->setChecked(!pattern->rad_auto);
    ui->mas1->setText(QString::number((double)pattern->m1));
    ui->mass2->setText(QString::number((double)pattern->m2));
    ui->q1->setText(QString::number((double)pattern->q1));
    ui->q2->setText(QString::number((double)pattern->q2));
    ui->p_line->setText(QString::number((double)pattern->p));
    ui->rad1->setText(QString::number((double)pattern->rad1));
    ui->rad2->setText(QString::number((double)pattern->rad2));
    ui->space->setText(QString::number((double)pattern->free_space));
    ui->speed->setText(QString::number((double)pattern->start_speed));
    ui->mx_speed_line->setText(QString::number(pattern->max_speed));

    printColors();
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

    ui->rad1->setText(ui->rad1->text());
    ui->rad2->setText(ui->rad2->text());
}

void GenerateWidget::fillPatternsList()
{
    ui->patterns_list->blockSignals(true);
    for(auto &var : pat_list)
    {
        ui->patterns_list->addItem(var.getName());
    }
    ui->patterns_list->blockSignals(false);
}

void GenerateWidget::blockColorsSignals(bool val)
{
    ui->black->blockSignals(val);
    ui->red->blockSignals(val);
    ui->green->blockSignals(val);
    ui->blue->blockSignals(val);
    ui->yellow->blockSignals(val);
    ui->all_colors->blockSignals(val);
}

void GenerateWidget::printColors()
{
    blockColorsSignals(true);

    ui->black->setChecked(false);
    ui->red->setChecked(false);
    ui->green->setChecked(false);
    ui->blue->setChecked(false);
    ui->yellow->setChecked(false);
    ui->all_colors->setChecked(false);

    bool sp = true;

    for(auto &var : pattern->colors)
    {
        if(var == Qt::black)
            ui->black->setChecked(true);
        else
            sp = false;
        if(var == Qt::red)
            ui->red->setChecked(true);
        else
            sp = false;
        if(var == Qt::green)
            ui->green->setChecked(true);
        else
            sp = false;
        if(var == Qt::blue)
            ui->blue->setChecked(true);
        else
            sp = false;
        if(var == Qt::yellow)
            ui->yellow->setChecked(true);
        else
            sp = false;

        if(sp)
            ui->all_colors->setChecked(true);
    }

    blockColorsSignals(false);
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
    pattern->m1 = arg1.toDouble();
}

void GenerateWidget::on_mass2_textEdited(const QString &arg1)
{
    pattern->m2 = arg1.toDouble();
}

void GenerateWidget::on_q1_textEdited(const QString &arg1)
{
    pattern->q1 = arg1.toDouble();
}

void GenerateWidget::on_q2_textEdited(const QString &arg1)
{
    pattern->q2 = arg1.toDouble();
}

void GenerateWidget::on_p_line_textEdited(const QString &arg1)
{
    pattern->p = arg1.toDouble();
}

void GenerateWidget::on_rad1_textEdited(const QString &arg1)
{
    pattern->rad1 = arg1.toDouble();
}

void GenerateWidget::on_rad2_textEdited(const QString &arg1)
{
    pattern->rad2 = arg1.toDouble();
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
        QMessageBox msgb;
        msgb.setWindowTitle("Предупреждение");
        msgb.setText("При таком количестве объектов [" + QString::number(*count) + "] работа программы может быть нестабильна!");
        msgb.setInformativeText("Начать генерацию?");
        msgb.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
        msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Question));
        msgb.setWindowIcon(windowIcon());

        int reg = msgb.exec();

        if(reg == QMessageBox::No)
            suc = false;
    }

    if(pattern->m1 == 0 || pattern->m2 == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Значение массы не может быть равно нулю!");
        suc = false;
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


    if(pattern->rad1 <= 0 || pattern->rad2 <= 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Значение радиуса не может быть равно нулю или быть меньше него!");
        suc = false;
    }

    if(pattern->rad1 > pattern->rad2)
    {
        QMessageBox::warning(this, "Предупреждение", "Верхний предел радиуса не может быть меньше нижнего!");
        suc = false;
    }

    if(*count == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Количество объектов не может быть равно нулю!");
        suc = false;
    }

     *(this->succes) = suc;

    if(suc)
    {
        if(pattern->rad1 < 0.1)
            pattern->rad1 = 0.1;
        if(pattern->rad2 < 0.1)
            pattern->rad2 = 0.1;
       close();
    }
}

void GenerateWidget::on_count_textEdited(const QString &arg1)
{
    *count = arg1.toInt();
}

void GenerateWidget::on_pushButton_2_clicked()
{
    *(this->succes) = false;
    close();
}

void GenerateWidget::on_patterns_list_currentRowChanged(int currentRow)
{
    if(!firstOut)
    {

    pattern->name = pat_list[currentRow].getName();

    pattern->m1 = pat_list[currentRow].getMass();
    pattern->m2 = pattern->m1;

    pattern->q1 = pat_list[currentRow].getQ();
    pattern->q2 = pattern->q1;

    pattern->p = pat_list[currentRow].getMass() / pat_list[currentRow].getRadius();
    pattern->rad_auto = false;
    pattern->rad1 = pat_list[currentRow].getRadius();
    pattern->rad2 = pattern->rad1;
    pattern->colors.clear();
    pattern->colors.push_back(pat_list[currentRow].getColor());

    FillFields();
    }

    else
    {
       ui->patterns_list->setCurrentRow(-1);
       firstOut = false;
    }
}

void GenerateWidget::on_mx_speed_line_textEdited(const QString &arg1)
{
    pattern->max_speed = arg1.toDouble();
}

void GenerateWidget::on_all_colors_stateChanged(int arg1)
{
    blockColorsSignals(true);

    ui->black->setChecked(ui->all_colors->isChecked());
    ui->red->setChecked(ui->all_colors->isChecked());
    ui->blue->setChecked(ui->all_colors->isChecked());
    ui->green->setChecked(ui->all_colors->isChecked());
    ui->yellow->setChecked(ui->all_colors->isChecked());

    blockColorsSignals(false);

    setColor();
}

void GenerateWidget::on_tangentum_speed_textEdited(const QString &arg1)
{
    pattern->tangentum_speed = ui->tangentum_speed->text().toDouble();
}

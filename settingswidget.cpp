#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(Settings *set, SimulationState *sim, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    ptr_settings = set;
    ptr_sim_state = sim;

    tmp_settings = *ptr_settings;

    background_color = *(sim->getBackcolorPointer());
    collision_mode = *(sim->getCollisionModePointer());

    ui->coll_mode1->setAutoExclusive(false);
    ui->coll_mode2->setAutoExclusive(false);

    ui->drop_settings->setToolTip("Установит настройки по умолчанию [Не отразится на текущей модели]");

    QRegExp valide_reg("\\-?\\d{1,}\\.?\\d{1,}");
    QValidator *validator = new QRegExpValidator(valide_reg, this);

    ui->g_line->setValidator(validator);
    ui->q_line->setValidator(validator);

    ui->sim_speed->setValidator(validator);

    fillFileds();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::fillFileds()
{
    ui->pause_after_create->blockSignals(true);
    ui->pause_after_create->setChecked(ptr_settings->SET_PAUSE_AFTER_CREATE);
    ui->pause_after_create->blockSignals(false);

    ui->pause_after_create->blockSignals(true);
    ui->pause_after_delete->setChecked(ptr_settings->SET_PAUSE_AFTER_DELETE);
    ui->pause_after_create->blockSignals(false);

    ui->pause_after_create->blockSignals(true);
    ui->pause_after_restart->setChecked(ptr_settings->SET_PAUSE_AFTER_RESTART);
    ui->pause_after_create->blockSignals(false);

    ui->pause_after_create->blockSignals(true);
    ui->back_camera->setChecked(ptr_settings->CAMERA_BUFFER_ENABLE);
    ui->pause_after_create->blockSignals(false);

    fillCollisionModeFields();
    fillBackcolorBox();

    ui->g_line->setText(QString::number(ptr_settings->G));
    ui->q_line->setText(QString::number(ptr_settings->K));

    ui->horizontalSlider->blockSignals(true);
    ui->spinBox->blockSignals(true);
    ui->horizontalSlider->setValue(ptr_settings->SCALE_SPEED*100);
    ui->spinBox->setValue(ptr_settings->SCALE_SPEED*100);
    ui->horizontalSlider->blockSignals(false);
    ui->spinBox->blockSignals(false);

    if(ui->delta2->isChecked())
    {
        ui->sim_speed->blockSignals(true);
        ui->sim_speed->setText(QString::number(ptr_settings->SIMULATION_SPEED));
        ui->sim_speed->blockSignals(false);
    }

    ui->delta1->setChecked(!ptr_settings->RENDER_MODE);
    ui->delta2->setChecked(ptr_settings->RENDER_MODE);

    ui->sim_speed->setEnabled(ptr_settings->RENDER_MODE);
    ui->sim_speed->setText(QString::number(ptr_settings->SIMULATION_SPEED));

    ui->fullscreen_box->blockSignals(true);
    ui->fullscreen_box->setChecked(ptr_settings->OPEN_FULLSCREEN);
    ui->fullscreen_box->blockSignals(false);
}

void SettingsWidget::fillCollisionModeFields()
{
    CollisionMode colllmode;

    if(ui->coll_mode1->isChecked())
        colllmode = collision_mode;
    else
        colllmode = tmp_settings.COLLISION_MODE;

    ui->merge_box->blockSignals(true);
    ui->elastic_box->blockSignals(true);
    ui->not_elastic_box->blockSignals(true);

    if(colllmode == CollisionMode::MERGE)
    {
        ui->merge_box->setChecked(true);
        ui->elastic_box->setChecked(false);
        ui->not_elastic_box->setChecked(false);
    }

    if(colllmode == CollisionMode::ELASTIC)
    {
        ui->merge_box->setChecked(false);
        ui->elastic_box->setChecked(true);
        ui->not_elastic_box->setChecked(false);
    }

    if(colllmode == CollisionMode::NOT_ELASTIC)
    {
        ui->merge_box->setChecked(false);
        ui->elastic_box->setChecked(false);
        ui->not_elastic_box->setChecked(true);
    }

    ui->merge_box->blockSignals(false);
    ui->elastic_box->blockSignals(false);
    ui->not_elastic_box->blockSignals(false);
}

void SettingsWidget::fillBackcolorBox()
{
    QColor backcolor;

    if(ui->color_mode1->isChecked())
        backcolor = background_color;
    else
        backcolor = tmp_settings.BACKGROUND_COLOR;

    ui->color_box->blockSignals(true);

    if(backcolor == Qt::white)
        ui->color_box->setCurrentIndex(0);

    if(backcolor == Qt::cyan)
        ui->color_box->setCurrentIndex(1);

    if(backcolor == Qt::darkGray)
        ui->color_box->setCurrentIndex(2);

    if(backcolor == Qt::darkRed)
        ui->color_box->setCurrentIndex(3);

    if(backcolor == Qt::darkGreen)
        ui->color_box->setCurrentIndex(4);

    if(backcolor == Qt::darkBlue)
        ui->color_box->setCurrentIndex(5);

    if(backcolor == Qt::darkYellow)
        ui->color_box->setCurrentIndex(6);

    ui->color_box->blockSignals(false);
}

void SettingsWidget::updateCollisionMode()
{
    int mode = 0;

    if(ui->merge_box->isChecked())
        mode = 0;

    if(ui->elastic_box->isChecked())
        mode = 1;

    if(ui->not_elastic_box->isChecked())
        mode = 2;


    if(ui->coll_mode1->isChecked())
    {
        setCollisionMode(mode);
    }

    else
    {
        tmp_settings.setCollisionMode(mode);
    }
}

void SettingsWidget::setBackgroundColor(int code)
{
    switch (code)
    {
    case 0:
        background_color = Qt::white;
        break;
    case 1:
        background_color = Qt::cyan;
        break;
    case 2:
        background_color = Qt::darkGray;
        break;
    case 3:
        background_color = Qt::darkRed;
        break;
    case 4:
        background_color = Qt::darkGreen;
        break;
    case 5:
        background_color = Qt::darkBlue;
        break;
    case 6:
        background_color = Qt::darkYellow;
        break;

    default:
        background_color = Qt::white;
        break;
    }
}

void SettingsWidget::setCollisionMode(int mode)
{
    switch (mode)
    {
    case 0:
        collision_mode = CollisionMode::MERGE;
        break;
    case 1:
        collision_mode = CollisionMode::ELASTIC;
        break;
    case 2:
        collision_mode = CollisionMode::NOT_ELASTIC;
        break;

    default:
        collision_mode = CollisionMode::MERGE;
        break;
    }
}


void SettingsWidget::on_color_mode1_clicked()
{
    fillBackcolorBox();
}

void SettingsWidget::on_coll_mode1_clicked()
{
    ui->coll_mode2->setChecked(false);
    fillCollisionModeFields();
}

void SettingsWidget::on_pause_after_create_clicked(bool checked)
{
    tmp_settings.SET_PAUSE_AFTER_CREATE = checked;
}

void SettingsWidget::on_pause_after_delete_clicked(bool checked)
{
    tmp_settings.SET_PAUSE_AFTER_DELETE = checked;
}

void SettingsWidget::on_pause_after_restart_clicked(bool checked)
{
    tmp_settings.SET_PAUSE_AFTER_RESTART = checked;
}

void SettingsWidget::on_back_camera_clicked(bool checked)
{
    tmp_settings.CAMERA_BUFFER_ENABLE = checked;
}

void SettingsWidget::on_g_line_textEdited(const QString &arg1)
{
    tmp_settings.G = arg1.toDouble();
}

void SettingsWidget::on_q_line_textEdited(const QString &arg1)
{
    tmp_settings.K = arg1.toDouble();
}

void SettingsWidget::on_horizontalSlider_valueChanged(int value)
{
    tmp_settings.SCALE_SPEED = (double)value / 100;
}

void SettingsWidget::on_spinBox_valueChanged(int arg1)
{
    tmp_settings.SCALE_SPEED = (double)arg1 / 100;
}

void SettingsWidget::on_color_mode2_clicked()
{
    fillBackcolorBox();
}

void SettingsWidget::on_color_box_currentIndexChanged(int index)
{
    if(ui->color_mode1->isChecked())
    {
        setBackgroundColor(index);
    }

    else
    {
        tmp_settings.setBackgroundColor(index);
    }
}

void SettingsWidget::on_coll_mode2_clicked()
{
    ui->coll_mode2->setChecked(false);
    fillCollisionModeFields();
}

void SettingsWidget::on_merge_box_clicked()
{
    updateCollisionMode();
}

void SettingsWidget::on_elastic_box_clicked()
{
    updateCollisionMode();
}

void SettingsWidget::on_not_elastic_box_clicked()
{
    updateCollisionMode();
}

void SettingsWidget::on_delta1_clicked(bool checked)
{
    tmp_settings.RENDER_MODE = !checked;
}

void SettingsWidget::on_delta2_clicked(bool checked)
{
    tmp_settings.RENDER_MODE = checked;
}

void SettingsWidget::on_sim_speed_textEdited(const QString &arg1)
{
    tmp_settings.SIMULATION_SPEED = arg1.toDouble();
}

void SettingsWidget::on_fullscreen_box_clicked(bool checked)
{
    tmp_settings.OPEN_FULLSCREEN = checked;
}

void SettingsWidget::on_drop_settings_clicked()
{
    tmp_settings.SetDefaultSettings();

    fillFileds();
}

void SettingsWidget::on_cancel_button_clicked()
{
    close();
}

void SettingsWidget::on_accept_button_clicked()
{
    *ptr_settings = tmp_settings;

    *(ptr_sim_state->getBackcolorPointer()) = background_color;
    *(ptr_sim_state->getCollisionModePointer()) = collision_mode;

    close();
}



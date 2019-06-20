#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QDialog>
#include "settings.h"
#include "simulationstate.h"
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QDialog
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent *event);


private:
    Settings *ptr_settings;
    SimulationState *ptr_sim_state;

    bool sucsess;

    Settings tmp_settings;

    QColor background_color;
    CollisionMode collision_mode;

public:
    explicit SettingsWidget(Settings *set, SimulationState *sim, QWidget *parent = nullptr);
    ~SettingsWidget();
    void fillFileds();
    void fillCollisionModeFields();
    void fillBackcolorBox();
    void updateCollisionMode();
    void setBackgroundColor(int code);
    void setCollisionMode(int mode);
    bool hasModifided();

private slots:
    void on_color_mode1_clicked();

    void on_coll_mode1_clicked();

    void on_pause_after_create_clicked(bool checked);

    void on_pause_after_delete_clicked(bool checked);

    void on_pause_after_restart_clicked(bool checked);

    void on_back_camera_clicked(bool checked);

    void on_g_line_textEdited(const QString &arg1);

    void on_q_line_textEdited(const QString &arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_color_mode2_clicked();

    void on_color_box_currentIndexChanged(int index);

    void on_coll_mode2_clicked();

    void on_merge_box_clicked();

    void on_elastic_box_clicked();

    void on_not_elastic_box_clicked();

    void on_delta1_clicked(bool checked);

    void on_delta2_clicked(bool checked);

    void on_sim_speed_textEdited(const QString &arg1);

    void on_fullscreen_box_clicked(bool checked);

    void on_drop_settings_clicked();

    void on_cancel_button_clicked();

    void on_accept_button_clicked();

    void on_SettingsWidget_destroyed();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H

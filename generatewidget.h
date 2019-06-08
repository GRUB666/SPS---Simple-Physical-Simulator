#ifndef GENERATEWIDGET_H
#define GENERATEWIDGET_H

#include <QDialog>
#include "phobject.h"

namespace Ui {
class GenerateWidget;
}

class GenerateWidget : public QDialog
{
    Q_OBJECT

private:
    GeneratePattern *pattern;
    bool *succes;
    int CamX, CamY;
    int* count;

public:
    GenerateWidget(QWidget *parent = nullptr, int CamX = 0, int CamY = 0, bool* success = nullptr, GeneratePattern* patternn = nullptr, int* count = 0);
    ~GenerateWidget();

private slots:
    void on_radprop_clicked();

    void on_radnprop_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_mas1_textEdited(const QString &arg1);

    void on_mass2_textEdited(const QString &arg1);

    void on_q1_textEdited(const QString &arg1);

    void on_q2_textEdited(const QString &arg1);

    void on_p_line_textEdited(const QString &arg1);

    void on_rad1_textEdited(const QString &arg1);

    void on_rad2_textEdited(const QString &arg1);

    void on_space_textEdited(const QString &arg1);

    void on_speed_textEdited(const QString &arg1);

    void on_cam1_clicked();

    void on_coord_clicked();

    void setColor();

    void on_pushButton_clicked();

    void on_count_textEdited(const QString &arg1);

private:
    void setRadiusMode(bool val);

private:
    Ui::GenerateWidget *ui;

signals:
    void SuccessFinish(GeneratePattern&, int);
};

#endif // GENERATEWIDGET_H

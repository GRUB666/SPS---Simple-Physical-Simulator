#ifndef GENERATEWIDGET_H
#define GENERATEWIDGET_H

#include <QDialog>
#include <QVector>

#include "phobject.h"
#include "documentationwidget.h"

//Этот класс требует серьёзного рефакторинга


namespace Ui {
class GenerateWidget;
}

class GenerateWidget : public QDialog
{
    Q_OBJECT

private:
    GeneratePattern *pattern;  //Указатель на шаблон
    QVector<PhObject> pat_list;
    bool *succes;
    int CamX, CamY;
    int* count;
    bool firstOut;

public:
    GenerateWidget(QVector<PhObject> pat_list, QWidget *parent = nullptr, bool* success = nullptr, GeneratePattern* patternn = nullptr, int* count = 0);
    ~GenerateWidget();

private:
    void FillFields();


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

    void on_pushButton_2_clicked();

    void on_patterns_list_currentRowChanged(int currentRow);

    void on_mx_speed_line_textEdited(const QString &arg1);

    void on_all_colors_stateChanged(int arg1);

    void on_tangentum_speed_textEdited(const QString &arg1);

    void on_label_linkActivated(const QString &link);

private:
    void setRadiusMode(bool val);
    void fillPatternsList();
    void blockColorsSignals(bool val);
    void printColors();

private:
    Ui::GenerateWidget *ui;

signals:
    void SuccessFinish(GeneratePattern&, int);
};

#endif // GENERATEWIDGET_H

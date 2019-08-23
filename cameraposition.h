#ifndef CAMERAPOSITION_H
#define CAMERAPOSITION_H

#include <QDialog>

namespace Ui {
class CameraPosition;
}

class CameraPosition : public QDialog
{
    Q_OBJECT

    double* cx;
    double* cy;

public:
    explicit CameraPosition(double* cPx, double* cPy, QWidget *parent = nullptr);
    ~CameraPosition();

private slots:
    void on_x_line_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_label_linkActivated(const QString &link);

private:
    Ui::CameraPosition *ui;
};

#endif // CAMERAPOSITION_H

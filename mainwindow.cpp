#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QDebug>

void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    if(pe->key() == 16777274)
    {
        setFullScreenMode(!FullScreenMode);
    }

    if(pe->key() == 16777216 && FullScreenMode)
    {
        setFullScreenMode(false);
    }

    qDebug() << pe->key();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(maximumSize());

    ui->statusBar->showMessage("Приложение загружено");              //Первоначальная инициализация приложения
    ui->add_button->setIcon(QIcon(QPixmap(":/img/img/plus.png")));
    ui->delete_button->setIcon(QIcon(QPixmap("://img/krest.png")));
    ui->delete_button->setEnabled(false);
    ui->to_center_button->setIcon(QIcon(QPixmap(":/img/center.png")));
    ui->to_center_button_2->setIcon(QIcon(QPixmap(":/img/drop_scale.png")));
    ui->checkpoint_button->setIcon(QIcon(QPixmap("://img/checkpoint.png")));

    ui->add_button->setToolTip("Создать новый объект");
    ui->delete_button->setToolTip("Удалить выбранный объект");
    ui->to_center_button->setToolTip("Переместить камеру в центр");
    ui->to_center_button_2->setToolTip("Установить масштаб на 100%");
    ui->checkpoint_button->setToolTip("Установить точку возврата");
    ui->addpattern_button->setToolTip("Создать шаблон");
    ui->deletepattern_button->setToolTip("Удалить выбранный шаблон");

    keyAdd = new QShortcut(this);
    keyAdd->setKey(Qt::CTRL + Qt::Key_A);

    keyDelete = new QShortcut(this);
    keyDelete->setKey(Qt::CTRL + Qt::Key_D);

    keyCenter = new QShortcut(this);
    keyCenter->setKey(Qt::CTRL + Qt::Key_C);

    keyScale = new QShortcut(this);
    keyScale->setKey(Qt::CTRL + Qt::Key_L);

    keyCheckPoint = new QShortcut(this);
    keyCheckPoint->setKey(Qt::CTRL + Qt::Key_F);


    ui->addpattern_button->setIcon(ui->add_button->icon());
    ui->deletepattern_button->setIcon(ui->delete_button->icon());
    current_pattern = 0;
    PhObject obj;
    obj.setName("Стандартный");
    Patterns.push_back(obj);
    updatePatternsList();
    setPatternMode(false);
    ui->viewport->setScrollSpeed(0.05);
    ui->scale_slider->setMaximum(ui->viewport->getScrollSpeed()*10000);
    ui->scale_slider->setMinimum(ui->viewport->getScrollSpeed()*100);
    changeScaleSlot(100);

    ui->restart_button->setIcon(QIcon(QPixmap("://img/restart.png")));
    timer = new QTimer;  //Таймер
    timer->setInterval(0);
    setPause(true);
    setFocus(-1);

    QRegExp valide_reg("\\-?\\d{1,}\\.?\\d{1,}");
    QValidator *validator = new QRegExpValidator(valide_reg, this);

    ui->mass_line->setValidator(validator);
    ui->q_line->setValidator(validator);
    ui->rad_line->setValidator(validator);
    ui->x_line->setValidator(validator);
    ui->y_line->setValidator(validator);
    ui->xs_line->setValidator(validator);
    ui->ys_line->setValidator(validator);
    ui->g_line->setValidator(validator);
    ui->k_line->setValidator(validator);

    //----Установка настроек

    SET_PAUSE_AFTER_CREATE = true;
    SET_PAUSE_AFTER_RESTART = true;
    FOLLOW_TO_FOCUS_OBJECT = true;
    FullScreenMode = false;
    G = 1;
    k = 1;
    setConstFields();

    //----------------------

    connect(timer, SIGNAL(timeout()), this, SLOT(ForceCalc()));            //Соединение сигналов и слотов
    connect(ui->pause_button, SIGNAL(clicked()), this, SLOT(setPause()));
    connect(ui->restart_button, SIGNAL(clicked()), this, SLOT(Restart()));
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addObject()));
    connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(deleteObject()));
    connect(ui->to_center_button, SIGNAL(clicked()), this, SLOT(moveCameraToCenter()));
    connect(ui->to_center_button_2, SIGNAL(clicked()), this, SLOT(dropCameraScale()));
    connect(ui->checkpoint_button, SIGNAL(clicked()), this, SLOT(setBuffer()));


    connect(ui->mass_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->q_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->rad_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->x_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->y_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->xs_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->ys_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->isMoveBox, SIGNAL(stateChanged(int)), this, SLOT(changeParameters()));
    connect(ui->color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));

    connect(ui->mass_mnog, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
    connect(ui->q_mnog, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
    connect(ui->scale_slider, SIGNAL(valueChanged(int)), this, SLOT(changeScaleSlot(int)));
    connect(ui->viewport, SIGNAL(whellScrolled(int)), this, SLOT(changeScaleSlot(int)));
    connect(ui->viewport, SIGNAL(camScrolled(int, int)), this, SLOT(changeCamLabel(int, int)));

    connect(ui->addpattern_button, SIGNAL(clicked()), this, SLOT(addPattern()));
    connect(ui->deletepattern_button, SIGNAL(clicked()), this, SLOT(deletePattern()));

    connect(keyAdd, SIGNAL(activated()), this, SLOT(addObject()));
    connect(keyDelete, SIGNAL(activated()), this, SLOT(deleteObject()));
    connect(keyCenter, SIGNAL(activated()), this, SLOT(moveCameraToCenter()));
    connect(keyScale, SIGNAL(activated()), this, SLOT(dropCameraScale()));
    connect(keyCheckPoint, SIGNAL(activated()), this, SLOT(setBuffer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ForceCalc()
{
    double F;
    double angle;
    double ax, ay;
    double distance;

    for(int i = 0; i < Objects.size(); i++)
    {
        ax = 0;
        ay = 0;

        if(!Objects[i].getStatic())
        {
        if(Objects[i].getMass() > 0)
        {
        for(int j = 0; j < Objects.size(); j++)
        {
            distance = getDistance(i, j);

            if(i != j && distance != 0)
            {
                F = G * Objects[i].getMass() * Objects[j].getMass()
                        / pow(distance, 2);

                F += -k * Objects[i].getQ() * Objects[j].getQ()
                        / pow(distance, 2);


                angle = atan2(Objects[j].getYPosition() - Objects[i].getYPosition(), Objects[j].getXPosition() - Objects[i].getXPosition());

                ax += F*cos(angle) / Objects[i].getMass();
                ay += F*sin(angle) / Objects[i].getMass();
            }
        }

        Objects[i].setXSpeed(Objects[i].getXSpeed() + ax);
        Objects[i].setYSpeed(Objects[i].getYSpeed() + ay);
        }

        Objects[i].setXPosition(Objects[i].getXPosition() + Objects[i].getXSpeed());
        Objects[i].setYPosition(Objects[i].getYPosition() + Objects[i].getYSpeed());
        }

        for(int j = 0; j < Objects.size(); j++)
        {
            if(j != i)
            {
                if (i == Objects.size() || j == Objects.size())
                        break;

                if(getDistance(i, j) < Objects[i].getRadius()*10 + Objects[j].getRadius()*10)
                {
                    double Vx, Vy;
                    double xc, yc;

                    if(Objects[i].getMass() >= Objects[j].getMass())
                    {
                        Vx = (Objects[i].getMass() * Objects[i].getXSpeed() + Objects[j].getMass() * Objects[j].getXSpeed()) / (Objects[i].getMass() + Objects[j].getMass());
                        Vy = (Objects[i].getMass() * Objects[i].getYSpeed() + Objects[j].getMass() * Objects[j].getYSpeed()) / (Objects[i].getMass() + Objects[j].getMass());

                        xc = (Objects[i].getMass() * Objects[i].getXPosition() + Objects[j].getMass() * Objects[j].getXPosition()) / (Objects[i].getMass() + Objects[j].getMass());
                        yc = (Objects[i].getMass() * Objects[i].getYPosition() + Objects[j].getMass() * Objects[j].getYPosition()) / (Objects[i].getMass() + Objects[j].getMass());

                        Objects[i].setPosition(xc, yc);
                        Objects[i].setRadius(sqrt(pow(Objects[i].getRadius(), 2) + pow(Objects[j].getRadius(), 2)));
                        Objects[i].setSpeed(Vx, Vy);
                        Objects[i].setMass(Objects[i].getMass() + Objects[j].getMass());
                        Objects[i].setQ(Objects[i].getQ() + Objects[j].getQ());


                        deleteObject(j);
                    }

                    else
                    {
                        Vx = (Objects[i].getMass() * Objects[i].getXSpeed() + Objects[j].getMass() * Objects[j].getXSpeed()) / (Objects[i].getMass() + Objects[j].getMass());
                        Vy = (Objects[i].getMass() * Objects[i].getYSpeed() + Objects[j].getMass() * Objects[j].getYSpeed()) / (Objects[i].getMass() + Objects[j].getMass());

                        xc = (Objects[i].getMass() * Objects[i].getXPosition() + Objects[j].getMass() * Objects[j].getXPosition()) / (Objects[i].getMass() + Objects[j].getMass());
                        yc = (Objects[i].getMass() * Objects[i].getYPosition() + Objects[j].getMass() * Objects[j].getYPosition()) / (Objects[i].getMass() + Objects[j].getMass());

                        Objects[j].setPosition(xc, yc);
                        Objects[j].setRadius(sqrt(pow(Objects[i].getRadius(), 2) + pow(Objects[j].getRadius(), 2)));
                        Objects[j].setSpeed(Vx, Vy);
                        Objects[j].setMass(Objects[i].getMass() + Objects[j].getMass());
                        Objects[j].setQ(Objects[i].getQ() + Objects[j].getQ());

                        deleteObject(i);
                    }
                }
            }
        }
    }

    if(current_index >= 0 && current_index < Objects.size())
    printToPanel(Objects[current_index], true);

    updateViewport();
}

void MainWindow::setPause()
{
    setPause(!isPause);
    if(isPause)
    ui->statusBar->showMessage("Симуляция приостановлена");
    else
    ui->statusBar->showMessage("Симуляция запущена");
}

void MainWindow::Restart()
{
    Objects = Buffer_Objects;
    setFocus();
    updateList();
    ui->delete_button->setEnabled(false);
    updateViewport();

    ui->statusBar->showMessage("Симуляция перезапущена с последней точки возврата");

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

void MainWindow::setFocus(int index)
{
    current_index = index;

    if(index < 0)
    {
        ui->Propertites_box->setEnabled(false);
        clearPrintPanel();
        ui->delete_button->setEnabled(false);
    }

    else
    {
        ui->delete_button->setEnabled(true);
        dropFocus();
        Objects[index].setFocus(true);
        updateViewport();
        ui->ListObjects->setCurrentRow(index);
        ui->Propertites_box->setEnabled(true);
        printToPanel(Objects[index]);
    }
}



void MainWindow::addObject()
{
    PhObject obj;
    if(current_pattern >= 0)
        obj = Patterns[current_pattern];
    else
    {
        obj.setName("Стандартный");
        Patterns.push_back(obj);
        updatePatternsList();
        current_pattern = 0;
        ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
        ui->deletepattern_button->setEnabled(true);
    }

    obj.setName("Object");

    obj.setPosition(ui->viewport->getCamX(), ui->viewport->getCamY());
    Objects.push_back(obj);
    updateList();
    setFocus(Objects.size() - 1);
    updateViewport();

    ui->statusBar->showMessage("Создан новый объект по шаблону: " + Patterns[current_pattern].getName());

    if(SET_PAUSE_AFTER_CREATE)
        setPause(true);
}

void MainWindow::deleteObject(int index)
{
    QString name;

    if(current_index < Objects.size())
    {

    if(index < 0)
    name = Objects[current_index].getName();

    if(index < 0)
    Objects.erase(Objects.begin() + current_index);
    else
    Objects.erase(Objects.begin() + index);


    if(Objects.size() == 0 || current_index == index)
    {
        setFocus(-1);
    }

    else if(index >= 0)
    {
       if(index < current_index)
       {
           current_index--;
           setFocus(current_index);
       }
    }

    else if(current_index >= Objects.size())
    {
        current_index = Objects.size() - 1;
        setFocus(current_index);

        updateViewport();
    }

    if(index < 0)
    {
        updateViewport();
        ui->statusBar->showMessage("Удалён объект: " + name);
    }


    updateList();
    }
}


void MainWindow::printToPanel(PhObject &toPrint, bool sp)
{
    double del;

    {
    if(!sp)
    {
    del = floor(log10(abs(toPrint.getMass())));
    if(del > 8)
        del = 8;

    if(del < -8)
        del = -8;

    if(toPrint.getMass() == 0)
        del = 0;


    ui->mass_line->setText(QString::number(toPrint.getMass() / pow(10, del)));
    ui->mass_mnog->blockSignals(true);
    ui->mass_mnog->setCurrentIndex(8 - del);
    ui->mass_mnog->blockSignals(false);

    del = floor(log10(abs(toPrint.getQ())));
    if(del > 8)
        del = 8;

    if(del < -8)
        del = -8;

    if(toPrint.getQ() == 0)
        del = 0;

    ui->q_line->setText(QString::number(toPrint.getQ() / pow(10, del)));
    ui->q_mnog->blockSignals(true);
    ui->q_mnog->setCurrentIndex(8 - del);
    ui->q_mnog->blockSignals(false);
    }

    if(pattern_mode)
    {
        ui->x_line->setText("Невозможно");
        ui->x_line->setEnabled(false);
    }

    else
    {
        ui->x_line->setEnabled(true);
        ui->x_line->setText(QString::number(toPrint.getXPosition()));
    }

    if(pattern_mode)
    {
        ui->y_line->setText("Невозможно");
        ui->y_line->setEnabled(false);
    }

    else
    {
        ui->y_line->setEnabled(true);
        ui->y_line->setText(QString::number(toPrint.getYPosition()));
    }

    if(pattern_mode)
    {
        ui->xs_line->setText("Невозможно");
        ui->xs_line->setEnabled(false);
    }

    else
    {
        ui->xs_line->setEnabled(true);
        ui->xs_line->setText(QString::number(toPrint.getXSpeed()));
    }

    if(pattern_mode)
    {
        ui->ys_line->setText("Невозможно");
        ui->ys_line->setEnabled(false);
    }

    else
    {
        ui->ys_line->setEnabled(true);
        ui->ys_line->setText(QString::number(toPrint.getYSpeed()));
    }
    }

    if(!sp)
    {
    ui->name_line->setText(toPrint.getName());
    ui->rad_line->setText(QString::number(toPrint.getRadius()));
    ui->isMoveBox->setChecked(toPrint.getStatic());

    setColorBox(toPrint);
    }
}

void MainWindow::setColorBox(PhObject &toSet)
{
    ui->color_box->blockSignals(true);

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

    ui->color_box->blockSignals(false);
}

void MainWindow::clearPrintPanel()
{
    ui->name_line->setText("");
    ui->mass_line->setText("");
    ui->q_line->setText("");
    ui->rad_line->setText("");
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

    ui->mass_mnog->blockSignals(true);
    ui->mass_mnog->setCurrentIndex(8);
    ui->mass_mnog->blockSignals(false);

    ui->q_mnog->blockSignals(true);
    ui->q_mnog->setCurrentIndex(8);
    ui->q_mnog->blockSignals(false);
}

void MainWindow::updateList()
{
    ui->ListObjects->blockSignals(true);
    ui->ListObjects->clear();
    ui->ListObjects->blockSignals(false);

    for(auto &var : Objects)
    {
        if(var.getName().isEmpty())
        {
            ui->ListObjects->addItem("Безымянный");
            var.setName("Безымянный");
        }

        else
            ui->ListObjects->addItem(var.getName());
    }
}

void MainWindow::dropFocus()
{
    for(auto &var : Objects)
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

int MainWindow::getPowerMnog(QComboBox * box)
{
    return 8 - box->currentIndex();
}

void MainWindow::updateViewport()
{
    ui->viewport->setPaintVector(Objects);
    ui->viewport->update();
}

double MainWindow::getDistance(int obj1, int obj2)
{
    return sqrt(pow(Objects[obj1].getXPosition() - Objects[obj2].getXPosition(), 2) + pow(Objects[obj1].getYPosition() - Objects[obj2].getYPosition(), 2));
}

void MainWindow::setConstFields()
{
    ui->g_line->setText(QString::number(G));
    ui->k_line->setText(QString::number(k));
}

void MainWindow::setFullScreenMode(bool val)
{
    FullScreenMode = val;

    if(FullScreenMode)
    {
        showFullScreen();
        ui->statusBar->showMessage("Включён полноэкранный режим. Для выхода нажмите F11 или Escape");
    }

    else
    {
        showNormal();
        ui->statusBar->showMessage("Выключен полноэкранный режим. Чтобы активировать полноэкранный режим нажмите F11");
    }
}


void MainWindow::changeParameters()
{
    if(!pattern_mode)
    {
    Objects[current_index].setMass(ui->mass_line->text().toDouble() * pow(10, getPowerMnog(ui->mass_mnog)));
    Objects[current_index].setQ(ui->q_line->text().toDouble() * pow(10, getPowerMnog(ui->q_mnog)));
    Objects[current_index].setRadius(ui->rad_line->text().toDouble());
    Objects[current_index].setPosition(ui->x_line->text().toDouble(), ui->y_line->text().toDouble());
    Objects[current_index].setSpeed(ui->xs_line->text().toDouble(), ui->ys_line->text().toDouble());
    Objects[current_index].setStatic(ui->isMoveBox->isChecked());
    Objects[current_index].setColor(getColorBox());

    if(Objects[current_index].getMass() == 0)
        ui->statusBar->showMessage("Внимание! Для объектов с нулевой массой не будут просчитываться силы");

    updateViewport();
    }

    else
    {
    Patterns[current_pattern].setMass(ui->mass_line->text().toDouble() * pow(10, getPowerMnog(ui->mass_mnog)));
    Patterns[current_pattern].setQ(ui->q_line->text().toDouble() * pow(10, getPowerMnog(ui->q_mnog)));
    Patterns[current_pattern].setRadius(ui->rad_line->text().toDouble());
    Patterns[current_pattern].setStatic(ui->isMoveBox->isChecked());
    Patterns[current_pattern].setColor(getColorBox());
    }

}


void MainWindow::on_name_line_textEdited(const QString &)
{
    if(!pattern_mode)
    {
        Objects[current_index].setName(ui->name_line->text());
        updateList();
    }

    else
    {
        Patterns[current_pattern].setName(ui->name_line->text());
        updatePatternsList();
        ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    }
}

void MainWindow::on_ListObjects_currentRowChanged(int currentRow)
{
    setPatternMode(false);
    setFocus(currentRow);
}

void MainWindow::addPattern()
{
    PhObject obj;
    obj.setName("Новый шаблон");
    Patterns.push_back(obj);
    setPatternMode(true);
    updatePatternsList();
    ui->Propertites_box->setEnabled(true);
    current_pattern = Patterns.size() - 1;
    ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    printToPanel(Patterns[Patterns.size() - 1]);
    ui->deletepattern_button->setEnabled(true);
    ui->statusBar->showMessage("Создан новый шаблон");
}

void MainWindow::deletePattern()
{
    if(current_pattern >= 0 && current_pattern < Patterns.size())
    {

    ui->statusBar->showMessage("Удалён шаблон: " + Patterns[current_pattern].getName());
    Patterns.erase(Patterns.begin() + current_pattern);
    if(Patterns.size() == 0)
    {
        int tmp = current_index;
        setFocus(-1);
        current_index = tmp;
        current_pattern = -1;
        QMessageBox::warning(this, "Предупреждение!", "Вы удалили все шаблоны, при создании нового объекта будет использоваться Стандартный шаблон");
        ui->deletepattern_button->setEnabled(false);
    }

    else if(current_pattern >= Patterns.size())
    {
        current_pattern = Patterns.size() - 1;
        printToPanel(Patterns[current_pattern]);
    }

    if(current_pattern < 0)
        ui->choosen_label->setText("Шаблонов нет");
    else
        ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());

    updatePatternsList();
    }
}

void MainWindow::updatePatternsList()
{
    ui->Patterns_list->blockSignals(true);
    ui->Patterns_list->clear();
    ui->Patterns_list->blockSignals(false);

    for(auto &var : Patterns)
    {
        if(var.getName().isEmpty())
        {
            ui->Patterns_list->addItem("Безымянный");
            var.setName("Безымянный");
        }
        else
            ui->Patterns_list->addItem(var.getName());
    }
}

void MainWindow::setPatternMode(bool val)
{
    pattern_mode = val;
    if(val)
        ui->Propertites_box->setTitle("Редактирование шаблона");
    else
        ui->Propertites_box->setTitle("Свойства объекта");
}

void MainWindow::on_Patterns_list_currentRowChanged(int currentRow)
{
    setPatternMode(true);
    current_pattern = currentRow;
    ui->Propertites_box->setEnabled(true);
    ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    printToPanel(Patterns[currentRow]);
}

void MainWindow::on_Patterns_list_clicked(const QModelIndex &index)
{
    setPatternMode(true);
    ui->Propertites_box->setEnabled(true);
    current_pattern = ui->Patterns_list->currentRow();
    ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    printToPanel(Patterns[current_pattern]);
}

void MainWindow::on_ListObjects_clicked(const QModelIndex &index)
{
    setPatternMode(false);
    setFocus(ui->ListObjects->currentRow());
}

void MainWindow::changeScaleSlot(int value)
{
    ui->scale_slider->setValue(value);
    ui->scale_label->setText(QString::number(value) + "%");
    ui->viewport->setScale((double)value / 100);
}

void MainWindow::changeCamLabel(int x, int y)
{
    ui->cam_pos_label->setText("Камера [" + QString::number(x) + "; " + QString::number(y) + "]");
}

void MainWindow::moveCameraToCenter()
{
    ui->viewport->setCamPos();
    changeCamLabel(0, 0);
    ui->statusBar->showMessage("Координаты камеры установлены на [0; 0]");
}

void MainWindow::dropCameraScale()
{
    changeScaleSlot(100);
    ui->statusBar->showMessage("Масштаб установлен на 100%");
}

void MainWindow::setBuffer()
{
    Buffer_Objects = Objects;
    ui->statusBar->showMessage("Установлена новая точка возврата");
}

void MainWindow::on_g_line_textEdited(const QString &arg1)
{
    G = arg1.toDouble();
}

void MainWindow::on_k_line_textEdited(const QString &arg1)
{
    k = arg1.toDouble();
}

void MainWindow::on_ListObjects_doubleClicked(const QModelIndex &index)
{
    if(current_index >= 0 || current_index < Objects.size())
    {
    ui->viewport->setCamX(Objects[current_index].getXPosition());
    ui->viewport->setCamY(Objects[current_index].getYPosition());
    }
}

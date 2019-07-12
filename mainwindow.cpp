#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QDebug>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(hasModifided())
    {
        int res;

        QMessageBox msgb;
        msgb.setWindowTitle("Закрытие программы");
        msgb.setText("Текущая модель была изменена");
        msgb.setInformativeText("Хотите сохранить её, прежде чем закрыть программу?");
        msgb.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Question));
        msgb.setWindowIcon(windowIcon());

        bool tmp_pause = isPause;
        setPause(true);

        res = msgb.exec();

        setPause(tmp_pause);

        if(res == QMessageBox::Save)
            saveModel();
        if(res == QMessageBox::Cancel)
            event->ignore();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    if(pe->key() == 16777216 && FullScreenMode) //16777216 - Ecs
    {
        setFullScreenMode(false);
    }

    if(pe->key() == Qt::Key_Delete)
    {
        deleteObject();
    }
}

MainWindow::MainWindow(QString version, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Установка настроек приложения--------------
    settings_way = QCoreApplication::applicationDirPath() + "/Settings/Settings.st";
    patterns_way = QCoreApplication::applicationDirPath() + "/Settings/Patterns.pt";

    loadSettings();

    newModel();

    Simulation_State = SimulationState(&G, &k, &Current_Collision_Mode,
                             ui->viewport->getBackgroundcolorPointer(),
                             ui->viewport->getPointerCamX(), ui->viewport->getPointerCamY(), &Objects);

    Simulation_State.setEmptyWay();

    this->FullScreenMode = !Programm_Settings.OPEN_FULLSCREEN;
    if(Programm_Settings.OPEN_FULLSCREEN)
        QTimer::singleShot(0, this, SLOT(changeFullScreenMode()));

    QTimer::singleShot(0, this, SLOT(openHello()));

    this->version = version;
    QString fs_str = "/Новая модель";
    setNewWindowTitle(fs_str);

    resize(maximumSize());
    setConstFields();
    //----------------------

    srand(time(0));
    setWindowIcon(QIcon(QPixmap(":/img/Mainico.png")));

    //Загрузка всех кнопок-------------------------------
    ui->statusBar->showMessage("Приложение загружено");
    ui->add_button->setIcon(QIcon(QPixmap(":/img/img/plus.png")));
    ui->delete_button->setIcon(QIcon(QPixmap("://img/krest.png")));
    ui->delete_button->setEnabled(false);
    ui->to_center_button->setIcon(QIcon(QPixmap(":/img/center.png")));
    ui->to_center_button_2->setIcon(QIcon(QPixmap(":/img/drop_scale.png")));
    ui->checkpoint_button->setIcon(QIcon(QPixmap("://img/checkpoint.png")));
    ui->set_center_button->setIcon(QIcon(QPixmap(":/img/setcenter.png")));
    ui->restart_button->setIcon(QIcon(QPixmap("://img/restart.png")));
    ui->nullspeed_button->setIcon(QIcon(QPixmap(":/img/nullspeed.png")));
    ui->clearall_button->setIcon(QIcon(QPixmap(":/img/clearall.png")));
    ui->addpattern_button->setIcon(ui->add_button->icon());
    ui->deletepattern_button->setIcon(ui->delete_button->icon());

    ui->add_button->setToolTip("Создать новый объект CTRL+A");
    ui->delete_button->setToolTip("Удалить выбранный объект CTRL+D");
    ui->to_center_button->setToolTip("Переместить камеру в центр CTRL+F");
    ui->to_center_button_2->setToolTip("Установить масштаб на 100% CTRL+L");
    ui->checkpoint_button->setToolTip("Установить точку возврата CTRL+C");
    ui->addpattern_button->setToolTip("Создать шаблон");
    ui->deletepattern_button->setToolTip("Удалить выбранный шаблон");
    ui->set_center_button->setToolTip("Сделать положение камеры началом координат CTRL+X");
    ui->pause_button->setToolTip("Запуск симуляции CTRL+TAB");
    ui->nullspeed_button->setToolTip("Сделать скорость выбранного объекта мировой скоростью CTRL+V");
    ui->clearall_button->setToolTip("Удалить все объекты CTRL+ALT+D");
    ui->restart_button->setToolTip("Запустить с последней точки возврата CTRL+R");

    //Установка горячих клавиш---------------------------------
    ui->add_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));

    ui->delete_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));

    ui->to_center_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));

    ui->to_center_button_2->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));

    ui->checkpoint_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    ui->restart_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    ui->pause_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab));

    ui->set_center_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));

    ui->follow_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_CapsLock));

    ui->nullspeed_button->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    ui->clearall_button->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_D));

    ui->generate_action->setShortcut(QKeySequence("CTRL+G"));

    ui->fullsreen_act->setShortcut(QKeySequence("F11"));

    ui->settings_act->setShortcut(QKeySequence("CTRL+K"));

    ui->create_new_model->setShortcut(QKeySequence("CTRL+N"));

    ui->open_model->setShortcut(QKeySequence("CTRL+O"));

    ui->save_current_model->setShortcut(QKeySequence("CTRL+S"));

    ui->doc_act->setShortcut(QKeySequence("F1"));
    //----------------------------------------------

    //Инициализация шаблонов
    loadPatterns();
    setPatternMode(false);
    //--------------------

    //Настройка параметров графического вывода
    ui->viewport->setScrollSpeed(Programm_Settings.SCALE_SPEED);
    ui->scale_slider->setMaximum(1000);
    ui->scale_slider->setMinimum(10);
    changeScaleSlot(100);
    //--------------------------

    //Таймер приложения, установка фокуса на -1
    timer = new QTimer(this);
    timer->setInterval(0);
    timer->start();
    setPause(true);
    setFocus(-1);
    //-----------------------


    //Валидаторы----------------------
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
    //---------------------------


    //Соединение сигналов и слотов--------------------------------------------

    //Сигналы кнопок-------------------
    connect(timer, SIGNAL(timeout()), this, SLOT(everyTickSlot()));
    connect(ui->pause_button, SIGNAL(clicked()), this, SLOT(setPause()));
    connect(ui->restart_button, SIGNAL(clicked()), this, SLOT(Restart()));
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addObject()));
    connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(deleteObject()));
    connect(ui->to_center_button, SIGNAL(clicked()), this, SLOT(moveCameraToCenter()));
    connect(ui->to_center_button_2, SIGNAL(clicked()), this, SLOT(dropCameraScale()));
    connect(ui->checkpoint_button, SIGNAL(clicked()), this, SLOT(setBuffer()));
    connect(ui->follow_button, SIGNAL(clicked()), this, SLOT(changeFollowSetting()));
    connect(ui->set_center_button, SIGNAL(clicked()), this, SLOT(makeCenter()));
    connect(ui->nullspeed_button, SIGNAL(clicked()), this, SLOT(makeNullSpeed()));
    connect(ui->addpattern_button, SIGNAL(clicked()), this, SLOT(addPattern()));
    connect(ui->deletepattern_button, SIGNAL(clicked()), this, SLOT(deletePattern()));
    connect(ui->clearall_button, SIGNAL(clicked()), this, SLOT(clearAllObjectsSlot()));
    //---------------------------------

    //Текстовые поля и ComboBox-----------
    connect(ui->mass_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->q_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->rad_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->x_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->y_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->xs_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->ys_line, SIGNAL(textEdited(QString)), this, SLOT(changeParameters()));
    connect(ui->isMoveBox, SIGNAL(stateChanged(int)), this, SLOT(changeParameters()));
    connect(ui->color_box, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
    //-------------------

    //Степени десятки---
    connect(ui->mass_mnog, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
    connect(ui->q_mnog, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters()));
    //------------------

    //Сигналы, связанные с графическим выводом---
    connect(ui->viewport, SIGNAL(whellScrolled(double)), this, SLOT(changeScaleSlot(double)));
    connect(ui->viewport, SIGNAL(camScrolled()), this, SLOT(updateCameraLabel()));
    connect(ui->scale_slider, SIGNAL(valueChanged(int)), this, SLOT(changeScaleSlot(int)));
    //-------------------------------------------

    //Дополнительные----------------
    connect(ui->generate_action, SIGNAL(triggered()), this, SLOT(OpenGenerateWidget()));
    connect(ui->fullsreen_act, SIGNAL(triggered()), this, SLOT(changeFullScreenMode()));
    connect(ui->settings_act, SIGNAL(triggered()), this, SLOT(OpenSettings()));
    connect(ui->create_new_model, SIGNAL(triggered()), this, SLOT(createNewModel()));
    connect(ui->save_current_model, SIGNAL(triggered()), this, SLOT(saveModel()));
    connect(ui->save_current_model_as, SIGNAL(triggered()), this, SLOT(saveModelAs()));
    connect(ui->open_model, SIGNAL(triggered()), this, SLOT(openModel()));
    connect(ui->doc_act, SIGNAL(triggered()), this, SLOT(openDocumentation()));
    //------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ForceCalc() //Рассчитывает силы и ускорения, действующие на все тела симуляции
{
    for(int i = 0; i < Objects.size() && Objects[i].getMass() != 0; i++)
    {
        long double ax = 0, ay = 0;
        long double distance;

        for(int j = 0; j < Objects.size(); j++)
        {
            if(i != j && getDistance(i, j) != 0) //Чтобы не делать рассчёт для одного и того же объекта
            {

            distance = getDistance(i, j);

            long double F;
            long double angle;

            F = G * Objects[i].getMass() * Objects[j].getMass()
                                    / std::pow(distance, 2);

            F += -k * Objects[i].getQ() * Objects[j].getQ()
                                    / std::pow(distance, 2);

            angle = std::atan2(Objects[j].getYPosition() - Objects[i].getYPosition(), Objects[j].getXPosition() - Objects[i].getXPosition());

            ax += F*std::cos(angle) / abs(Objects[i].getMass());
            ay += F*std::sin(angle) / abs(Objects[i].getMass());

            }
        }

        Objects[i].setXAccel(ax);
        Objects[i].setYAccel(ay);
    }
}

void MainWindow::MoveObjects()
{
    if(!Programm_Settings.RENDER_MODE)
        delta = 1;

    for(int i = 0; i < Objects.size(); i++)
    {
        if(Objects[i].getStatic())
        {
            Objects[i].setAccel(0, 0); //Сбрасываем все харакетристики движения, если тело статично (Сброс скорости нужен на случай, если она была установлена вручную)
            Objects[i].setSpeed(0, 0);
        }

        Objects[i].setXSpeed(Objects[i].getXSpeed() + Objects[i].getXAccel() * delta * Programm_Settings.SIMULATION_SPEED);
        Objects[i].setYSpeed(Objects[i].getYSpeed() + Objects[i].getYAccel() * delta * Programm_Settings.SIMULATION_SPEED);

        Objects[i].setXPosition(Objects[i].getXPosition() + Objects[i].getXSpeed() * delta * Programm_Settings.SIMULATION_SPEED);
        Objects[i].setYPosition(Objects[i].getYPosition() + Objects[i].getYSpeed() * delta * Programm_Settings.SIMULATION_SPEED);

        if(follow_to_focus_object && Objects[i].getFocus())//Слежка за выбранным объектом если это необходимо
           followToObject(Objects[i]);
    }
}

void MainWindow::CollisionsCheck()
{
    bool ellastic_check_array[Objects.size()];

    for(int i = 0; i < Objects.size(); i++) { ellastic_check_array[i] = false; }


    for(int i = 0; i < Objects.size(); i++)
    {
        for(int j = 0; j < Objects.size(); j++)
        {
            if(i >= Objects.size() || j >= Objects.size())
                break;

            if(getDistanceOfCollision(i, j) >= 0 && i != j) //Если расстояние между центрами меньше либо равно сумме их радиусов
            {
                if(Current_Collision_Mode == CollisionMode::MERGE)
                {
                    long double Vx, Vy;
                    long double xc, yc;

                    Vx = (Objects[i].getMass() * Objects[i].getXSpeed() + Objects[j].getMass() * Objects[j].getXSpeed()) / (Objects[i].getMass() + Objects[j].getMass());
                    Vy = (Objects[i].getMass() * Objects[i].getYSpeed() + Objects[j].getMass() * Objects[j].getYSpeed()) / (Objects[i].getMass() + Objects[j].getMass());

                    xc = (Objects[i].getMass() * Objects[i].getXPosition() + Objects[j].getMass() * Objects[j].getXPosition()) / (Objects[i].getMass() + Objects[j].getMass());
                    yc = (Objects[i].getMass() * Objects[i].getYPosition() + Objects[j].getMass() * Objects[j].getYPosition()) / (Objects[i].getMass() + Objects[j].getMass());

                    if(abs(Objects[i].getMass()) >= abs(Objects[j].getMass()))
                    {
                       if(Objects[i].getMass() == Objects[j].getMass() && abs(Objects[i].getQ()) == abs(Objects[j].getQ()) && Objects[i].getQ() != 0)
                          Objects[i].setColor(Qt::black);

                       Objects[i].setPosition(xc, yc);
                       Objects[i].setRadius(std::sqrt(std::pow(Objects[i].getRadius(), 2) + std::pow(Objects[j].getRadius(), 2)));
                       Objects[i].setSpeed(Vx, Vy);
                       Objects[i].setMass(Objects[i].getMass() + Objects[j].getMass());
                       Objects[i].setQ(Objects[i].getQ() + Objects[j].getQ());

                       deleteObject(j);
                     }

                    else
                    {
                        if(Objects[i].getMass() == Objects[j].getMass() && abs(Objects[i].getQ()) == abs(Objects[j].getQ()) && Objects[i].getQ() != 0)
                           Objects[j].setColor(Qt::black);

                        Objects[j].setPosition(xc, yc);
                        Objects[j].setRadius(std::sqrt(std::pow(Objects[i].getRadius(), 2) + std::pow(Objects[j].getRadius(), 2)));
                        Objects[j].setSpeed(Vx, Vy);
                        Objects[j].setMass(Objects[i].getMass() + Objects[j].getMass());
                        Objects[j].setQ(Objects[i].getQ() + Objects[j].getQ());

                        deleteObject(i);
                    }
                }

                if(Current_Collision_Mode == CollisionMode::ELASTIC)
                {
                    long double rad_speed_1, rad_speed_2;
                    long double dist_time;
                    long double alpha1 = std::atan2(Objects[j].getYPosition() - Objects[i].getYPosition(), Objects[j].getXPosition() - Objects[i].getXPosition());
                    //long double alpha2 = std::atan2(Objects[i].getYPosition() - Objects[j].getYPosition(), Objects[i].getXPosition() - Objects[j].getXPosition());
                    long double V1_vector, V2_vector;
                    long double normal_speed_1, normal_speed_2;
                    long double v_alpha1 = std::atan2(Objects[i].getYSpeed(), Objects[i].getXSpeed());
                    long double v_alpha2 = std::atan2(Objects[j].getYSpeed(), Objects[j].getXSpeed());

                    V1_vector = std::sqrt(std::pow(Objects[i].getXSpeed(), 2) + std::pow(Objects[i].getYSpeed(), 2));
                    V2_vector = std::sqrt(std::pow(Objects[j].getXSpeed(), 2) + std::pow(Objects[j].getYSpeed(), 2));

                    Objects[i].setXPosition(Objects[i].getXPosition() - Objects[i].getXSpeed()); //Двигаем назад
                    Objects[i].setYPosition(Objects[i].getYPosition() - Objects[i].getYSpeed());

                    Objects[j].setXPosition(Objects[j].getXPosition() - Objects[j].getXSpeed());
                    Objects[j].setYPosition(Objects[j].getYPosition() - Objects[j].getYSpeed());

                    long double dist = getDistanceOfCollision(i, j);

                    long double tmp_rad1 = rad_speed_1 = V1_vector * std::cos(alpha1 - v_alpha1);
                    long double tmp_rad2 = rad_speed_2 = V2_vector * std::cos(alpha1 - v_alpha2);

                    dist_time = (abs(tmp_rad1) + abs(tmp_rad2)) / dist;

                    normal_speed_1 = V1_vector * std::cos(PI * 0.5 - (alpha1 - v_alpha1));
                    normal_speed_2 = V2_vector * std::cos(PI * 0.5 - (alpha1 - v_alpha2));

                    rad_speed_1 = (2*Objects[j].getMass() * tmp_rad2 + tmp_rad1*(Objects[i].getMass() - Objects[j].getMass())) / (Objects[i].getMass() + Objects[j].getMass());

                    rad_speed_2 = (2*Objects[i].getMass() * tmp_rad1 + tmp_rad2*(Objects[j].getMass() - Objects[i].getMass())) / (Objects[i].getMass() + Objects[j].getMass());

                    V1_vector = std::sqrt(std::pow(rad_speed_1, 2) + std::pow(normal_speed_1, 2));
                    V2_vector = std::sqrt(std::pow(rad_speed_2, 2) + std::pow(normal_speed_2, 2));

                    if(!ellastic_check_array[i])
                    {
                    Objects[i].setXSpeed(V1_vector * std::cos(alpha1 - std::atan2(normal_speed_1, rad_speed_1)));
                    Objects[i].setYSpeed(V1_vector * std::sin(alpha1 - std::atan2(normal_speed_1, rad_speed_1)));

                    if(dist <= 0)
                    {
                        Objects[i].setXPosition(Objects[i].getXPosition() + tmp_rad1*std::cos(alpha1) * dist_time);
                        Objects[i].setYPosition(Objects[i].getYPosition() + tmp_rad1*std::sin(alpha1) * dist_time);
                    }

                    }

                    if(!ellastic_check_array[j])
                    {
                    Objects[j].setXSpeed(V2_vector * std::cos(alpha1 - std::atan2(normal_speed_2, rad_speed_2)));
                    Objects[j].setYSpeed(V2_vector * std::sin(alpha1 - std::atan2(normal_speed_2, rad_speed_2)));

                    if(dist <= 0)
                    {
                        Objects[j].setXPosition(Objects[j].getXPosition() - tmp_rad2*std::cos(alpha1) * dist_time);
                        Objects[j].setYPosition(Objects[j].getYPosition() - tmp_rad2*std::sin(alpha1) * dist_time);
                    }


                    }

                    ellastic_check_array[i] = ellastic_check_array[j] = true;
                }

                if(Current_Collision_Mode == CollisionMode::NOT_ELASTIC)
                {
                    long double rad_speed_1, rad_speed_2;
                    long double dist_time;
                    long double alpha1 = std::atan2(Objects[j].getYPosition() - Objects[i].getYPosition(), Objects[j].getXPosition() - Objects[i].getXPosition());
                    //long double alpha2 = std::atan2(Objects[i].getYPosition() - Objects[j].getYPosition(), Objects[i].getXPosition() - Objects[j].getXPosition());
                    long double V1_vector, V2_vector;
                    long double normal_speed_1, normal_speed_2;
                    long double v_alpha1 = std::atan2(Objects[i].getYSpeed(), Objects[i].getXSpeed());
                    long double v_alpha2 = std::atan2(Objects[j].getYSpeed(), Objects[j].getXSpeed());


                    V1_vector = std::sqrt(std::pow(Objects[i].getXSpeed(), 2) + std::pow(Objects[i].getYSpeed(), 2));
                    V2_vector = std::sqrt(std::pow(Objects[j].getXSpeed(), 2) + std::pow(Objects[j].getYSpeed(), 2));

                    Objects[i].setXPosition(Objects[i].getXPosition() - Objects[i].getXSpeed()); //Двигаем назад
                    Objects[i].setYPosition(Objects[i].getYPosition() - Objects[i].getYSpeed());

                    Objects[j].setXPosition(Objects[j].getXPosition() - Objects[j].getXSpeed());
                    Objects[j].setYPosition(Objects[j].getYPosition() - Objects[j].getYSpeed());

                    long double dist = getDistanceOfCollision(i, j);

                    long double tmp_rad1 = rad_speed_1 = V1_vector * std::cos(alpha1 - v_alpha1);
                    long double tmp_rad2 = rad_speed_2 = V2_vector * std::cos(alpha1 - v_alpha2);

                    dist_time = (abs(tmp_rad1) + abs(tmp_rad2)) / dist;

                    normal_speed_1 = V1_vector * std::cos(PI * 0.5 - (alpha1 - v_alpha1));
                    normal_speed_2 = V2_vector * std::cos(PI * 0.5 - (alpha1 - v_alpha2));

                    rad_speed_1 = rad_speed_2 = tmp_rad1 + tmp_rad2;

                    V1_vector = std::sqrt(std::pow(rad_speed_1, 2) + std::pow(normal_speed_1, 2));
                    V2_vector = std::sqrt(std::pow(rad_speed_2, 2) + std::pow(normal_speed_2, 2));

                    if(!ellastic_check_array[i])
                    {
                    Objects[i].setXSpeed(V1_vector * std::cos(alpha1 - std::atan2(normal_speed_1, rad_speed_1)));
                    Objects[i].setYSpeed(V1_vector * std::sin(alpha1 - std::atan2(normal_speed_1, rad_speed_1)));

                    if(dist <= 0)
                    {
                        Objects[i].setXPosition(Objects[i].getXPosition() + tmp_rad1*std::cos(alpha1) * dist_time);
                        Objects[i].setYPosition(Objects[i].getYPosition() + tmp_rad1*std::sin(alpha1) * dist_time);
                    }

                    }

                    if(!ellastic_check_array[j])
                    {
                    Objects[j].setXSpeed(V2_vector * std::cos(alpha1 - std::atan2(normal_speed_2, rad_speed_2)));
                    Objects[j].setYSpeed(V2_vector * std::sin(alpha1 - std::atan2(normal_speed_2, rad_speed_2)));

                    if(dist <= 0)
                    {
                        Objects[j].setXPosition(Objects[j].getXPosition() - tmp_rad2*std::cos(alpha1) * dist_time);
                        Objects[j].setYPosition(Objects[j].getYPosition() - tmp_rad2*std::sin(alpha1) * dist_time);
                    }

                    }

                    ellastic_check_array[i] = ellastic_check_array[j] = true;
                }
            }
        }
    }
}

void MainWindow::everyTickSlot()
{
    if(!isPause)
    {
        auto begin = std::chrono::steady_clock::now();
        ForceCalc();
        auto end = std::chrono::steady_clock::now();

        auto elapsed_mc = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        delta += elapsed_mc.count();
        MoveObjects();
        CollisionsCheck();

        if(current_index >= 0 && current_index < Objects.size())
        printToPanel(Objects[current_index], false);
    }

    updateViewport();
}


void MainWindow::setPause()
{
    setPause(!isPause);
    if(isPause)
    {
        ui->statusBar->showMessage("Симуляция приостановлена");
    }

    else
    {
        ui->statusBar->showMessage("Симуляция запущена");
    }

}

void MainWindow::Restart()
{
    Objects = Buffer_Objects;
    if(Programm_Settings.CAMERA_BUFFER_ENABLE)
    {
        ui->viewport->setCamX(BCamX);
        ui->viewport->setCamY(BCamY);

        updateCameraLabel();
    }
    setFocus();
    updateList();
    ui->delete_button->setEnabled(false);
    updateViewport();

    ui->statusBar->showMessage("Симуляция перезапущена с последней точки возврата");

    if(Programm_Settings.SET_PAUSE_AFTER_RESTART)
        setPause(true);
}

void MainWindow::setPause(bool val)
{
    isPause = val;

    if(isPause)
    {
        ui->viewport->setStyleSheet("#viewport { border: 5px solid red; }");
        ui->pause_button->setIcon(QIcon(QPixmap("://img/start.png")));
        //timer->stop();
        ui->pause_button->setToolTip("Запуск симуляции CTRL+TAB");
    }

    else
    {
        ui->viewport->setStyleSheet("#viewport { border: 0px solid black; }");
        ui->pause_button->setIcon(QIcon(QPixmap("://img/stop.png")));
        //timer->start(0);
        ui->pause_button->setToolTip("Остановить симуляцию CTRL+TAB");
    }
}

void MainWindow::setFocus(int index /*= -1*/)
{
    current_index = index;

    if(index < 0)
    {
        ui->Propertites_box->setEnabled(false);
        dropFocus();
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

    if(checkIndexValid(current_pattern, Patterns))
    {
        obj = Patterns[current_pattern];
    }

    else //В случае отсутствия шаблона
    {
        addStandartPattern();
        updatePatternsList();
        ui->statusBar->showMessage("Для создания нового объекта был использован Стандартный шаблон, так как другие шаблоны отсутствуют");
        obj = Patterns[current_pattern];
    }

    obj.setName(obj.getName() + " " + QString::number(Objects.size() + 1));
    obj.setPosition(ui->viewport->getCamX(), ui->viewport->getCamY());
    Objects.push_back(obj);
    updateList();
    setFocus(Objects.size() - 1);
    updateViewport();

    ui->statusBar->showMessage("Создан новый объект по шаблону: " + Patterns[current_pattern].getName());

    if(Programm_Settings.SET_PAUSE_AFTER_CREATE)
        setPause(true);
}

void MainWindow::deleteObject(int index /*= - 1*/)
{
    QString name;

    if(checkIndexValid(index, Objects))
    {
        Objects.erase(Objects.begin() + index);

        if(current_index == index)
            setFocus();

        else if(current_index > index)
        {
            current_index--;
            setFocus(current_index);
        }
    }

    else if(checkIndexValid(current_index, Objects))
    {
        name = Objects[current_index].getName();
        Objects.erase(Objects.begin() + current_index);

        if(current_index >= Objects.size())
        {
            current_index--;
            setFocus(current_index);

            ui->statusBar->showMessage("Удалён объект: " + name);
        }

        setFocus(current_index);

        updateViewport();

        if(Programm_Settings.SET_PAUSE_AFTER_DELETE) //Такая конструкция нужна, чтобы не засорять statusBar
            setPause(true);

    }

    updateList();
}

void MainWindow::printToPanel(PhObject &toPrint, bool sp)//sp - special. sp == true - Были изменения массы или заряда, false - не были
{
    long double del;

    if(!sp) //Вывод статичной информации
    {
    del = floor(log10(abs(toPrint.getMass())));
    if(del > 8)
        del = 8;

    if(del < -8)
        del = -8;

    if(toPrint.getMass() == 0)
        del = 0;

    ui->mass_line->setText(QString::number(double(toPrint.getMass() / pow(10, del))));
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

    ui->q_line->setText(QString::number(double(toPrint.getQ() / pow(10, del))));
    ui->q_mnog->blockSignals(true);
    ui->q_mnog->setCurrentIndex(8 - del);
    ui->q_mnog->blockSignals(false);


    ui->name_line->setText(toPrint.getName());
    ui->rad_line->setText(QString::number(double(toPrint.getRadius())));
    ui->isMoveBox->blockSignals(true);
    ui->isMoveBox->setChecked(toPrint.getStatic());
    ui->isMoveBox->blockSignals(false);

    setColorBox(toPrint);
    }


    if(pattern_mode)
    {
        ui->x_line->setText("Невозможно");
        ui->x_line->setEnabled(false);

        ui->y_line->setText("Невозможно");
        ui->y_line->setEnabled(false);

        ui->xs_line->setText("Невозможно");
        ui->xs_line->setEnabled(false);

        ui->ys_line->setText("Невозможно");
        ui->ys_line->setEnabled(false);
    }

    else
    {
        ui->x_line->setEnabled(true);
        ui->x_line->setText(QString::number(double(toPrint.getXPosition())));

        ui->y_line->setEnabled(true);
        ui->y_line->setText(QString::number(double(toPrint.getYPosition())));

        ui->xs_line->setEnabled(true);
        ui->xs_line->setText(QString::number(double(toPrint.getXSpeed())));

        ui->ys_line->setEnabled(true);
        ui->ys_line->setText(QString::number(double(toPrint.getYSpeed())));
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

    ui->count_lab->setText("Объектов: " + QString::number(Objects.size()));

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

QColor getColor(int code)
{
    switch (code)
    {
    case 0:
        return Qt::black;
        break;
    case 1:
        return Qt::red;
        break;
    case 2:
        return Qt::green;
        break;
    case 3:
        return Qt::blue;
        break;
    case 4:
        return Qt::yellow;
        break;

    default:
        return Qt::black;
        break;
    }

    return Qt::black;
}

QColor MainWindow::getColorBox()
{
    return getColor(ui->color_box->currentIndex());
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

long double MainWindow::getDistance(int obj1, int obj2)
{
    return std::sqrt(std::pow(Objects[obj1].getXPosition() - Objects[obj2].getXPosition(), 2) + std::pow(Objects[obj1].getYPosition() - Objects[obj2].getYPosition(), 2));
}

long double MainWindow::getDistance(double px, double py, PhObject &obj)
{
    return std::sqrt(std::pow(px - obj.getXPosition(), 2) + std::pow(py - obj.getYPosition(), 2));
}

void MainWindow::setConstFields() //Устанавливает значение констант в полях
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

void MainWindow::changeFullScreenMode()
{
    setFullScreenMode(!isFullScreen());
}

void MainWindow::clearAllObjectsSlot()
{
    bool tmpPause = isPause;

    if(Objects.size() > 0)
    {
    QMessageBox msgb;
    msgb.setWindowTitle("Внимание!");
    msgb.setText("Вы собираетесь удалить все объекты в текущей модели");
    msgb.setInformativeText("Вы действительно хотите удалить все объекты?");
    msgb.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Question));
    msgb.setWindowIcon(windowIcon());

    setPause(true);
    int result = msgb.exec();
    setPause(tmpPause);

    if(result == QMessageBox::Ok)
    {
        clearObjectsVector();
        ui->statusBar->showMessage("Все объекты были удалены");
        setFocus();
    }
    }

    else
    {
        ui->statusBar->showMessage("Невозможно удалить объекты, так как их список пуст!");
    }
}

void MainWindow::OpenSettings()
{
    bool succes;

    SettingsWidget *win = new SettingsWidget(&Programm_Settings, &Simulation_State, &succes);
    win->setWindowIcon(windowIcon());

    win->exec();

    if(succes)
        ui->statusBar->showMessage("Настройки были успешно изменены");
    else
        ui->statusBar->showMessage("Настройки не были изменены");

    saveSettings();

    ui->viewport->setScrollSpeed(Programm_Settings.SCALE_SPEED);

    updateViewport();
}

//Слот, который создаёт новую модель
void MainWindow::createNewModel()
{
    if(hasModifided())
    {
        int res;

        QMessageBox msgb;
        msgb.setWindowTitle("Создание новой модели");
        msgb.setText("Текущая модель была изменена");
        msgb.setInformativeText("Хотите сохранить её?");
        msgb.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Question));
        msgb.setWindowIcon(windowIcon());
        res = msgb.exec();

        if(res == QMessageBox::Save)
            saveModel();

        if(res == QMessageBox::Cancel)
            return;

        newModel();
    }

    else
        newModel();
}

void MainWindow::save()
{
    if(Simulation_State.WriteDataInFile())
    {
        ui->statusBar->showMessage("Файл успешно сохранён");
        Save_Buffer_Objects = Objects;
        QString way = Simulation_State.getFileWay();
        setNewWindowTitle(way);
    }

    else
        ui->statusBar->showMessage("Файл не сохранён");
}

void MainWindow::open(QString prev_way /*= ""*/)
{
    int result = Simulation_State.ReadDataFromFile();

    if(result == 0)
    {
        ui->statusBar->showMessage("Файл успешно открыт");
        Save_Buffer_Objects = Objects;
        Buffer_Objects = Objects;
        BCamX = ui->viewport->getCamX();
        BCamY = ui->viewport->getCamY();
        updateCameraLabel();
        QString way = Simulation_State.getFileWay();
        setNewWindowTitle(way);
    }

    if(result == 1) //Ошибка чтения файла
    {
        QMessageBox msgb;
        msgb.setWindowTitle("Ошибка чтения файла");
        msgb.setText("Некорретный файл модели!");
        msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Warning));
        msgb.setWindowIcon(windowIcon());

        ui->statusBar->showMessage("Ошибка открытия файла!");
        msgb.exec();

        Simulation_State.setFileWay(prev_way);
    }

    if(result == 2) //Путь не существует (Путь не выбран)
    {
        ui->statusBar->showMessage("Отркытие файла отменено");
        Simulation_State.setFileWay(prev_way);
    }
}

void MainWindow::setNewWindowTitle(QString &way)
{
    QString name_string = "";
    int i = way.size() - 1;

    while(i >= 0 && way[i] != '/')
    {
        name_string.push_front(way[i]);
        i--;
    }

    setWindowTitle(name_string + " - Simple Physical Simulator (SPS)" + " " + version);
}

void MainWindow::savePatterns()
{
    QFile patterns_file(patterns_way);

    QTextStream writer(&patterns_file);

    QDir dir(QCoreApplication::applicationDirPath());

    if(!dir.exists("Settings"))
        dir.mkdir("Settings");

    if(patterns_file.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        writer.setRealNumberPrecision(20);

        writer << Patterns.size() << endl;

        for(auto &var : Patterns)
        {
            writer << var.getName() << endl;
            writer << (double)var.getMass() << endl;
            writer << (double)var.getQ() << endl;
            writer << (double)var.getRadius() << endl;

            {
                if(var.getColor() == Qt::black)
                    writer << 0 << endl;
                if(var.getColor() == Qt::red)
                    writer << 1 << endl;
                if(var.getColor() == Qt::green)
                    writer << 2 << endl;
                if(var.getColor() == Qt::blue)
                    writer << 3 << endl;
                if(var.getColor() == Qt::yellow)
                    writer << 4 << endl;
            }

            writer << var.getStatic() << endl;
        }


        patterns_file.close();
    }
}

void MainWindow::loadPatterns()
{
    QFile patterns_file(patterns_way);
    QTextStream reader(&patterns_file);
    QString buffer;

    QRegExp float_reg("\\-?\\d{1,}\\.?\\d{1,}e?\\-?\\d{1,}");
    float_reg.setPatternSyntax(QRegExp::RegExp);

    QRegExp int_reg("\\-?\\d{1,}");
    int_reg.setPatternSyntax(QRegExp::RegExp);

    bool isCorrect = true;

    int count;
    int color;

    if(patterns_file.exists())
    {
        if(patterns_file.open(QIODevice::Text | QIODevice::ReadOnly))
        {
        reader.setRealNumberPrecision(20);

        buffer = reader.readLine();
        count = buffer.toInt();
        isCorrect &= (float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer)) && count >= 0;
        Patterns.resize(count);

        for(int i = 0; i < count && isCorrect; i++)
        {
            buffer = reader.readLine();
            Patterns[i].setName(buffer);

            buffer = reader.readLine();
            Patterns[i].setMass(buffer.toDouble());
            isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            Patterns[i].setQ(buffer.toDouble());
            isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            Patterns[i].setRadius(buffer.toDouble());
            isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            color = buffer.toInt();
            isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            buffer = reader.readLine();
            Patterns[i].setStatic(buffer.toInt());
            isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

            switch (color)
            {
            case 0:
                Patterns[i].setColor(Qt::black);
                break;
            case 1:
                Patterns[i].setColor(Qt::red);
                break;
            case 2:
                Patterns[i].setColor(Qt::green);
                break;
            case 3:
                Patterns[i].setColor(Qt::blue);
                break;
            case 4:
                Patterns[i].setColor(Qt::yellow);
                break;

            default:
                Patterns[i].setColor(Qt::black);
                break;
            }
        }

        patterns_file.close();

        if(!isCorrect)
        {
            QMessageBox msgb;
            msgb.setWindowTitle("Предупреждение");
            msgb.setText("Файл шаблонов был повреждён");
            msgb.setInformativeText("Шаблоны будут загружены по умолчанию");
            msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Information));
            msgb.setWindowIcon(windowIcon());

            msgb.exec();

            Patterns.clear();
            addStandartPattern();
            savePatterns();
        }

        if(Patterns.isEmpty())
        {
            addStandartPattern();
            savePatterns();
        }

        }

        else
        {
            QMessageBox::warning(this, "Ошибка", "Не доступа к файлу шаблонов, программа может работать некорректно (Шаблоны будут загружены по умолчанию)");
            addStandartPattern();
        }
    }

    else
    {
        addStandartPattern();
        savePatterns();
    }

    current_pattern = 0;
    updatePatternsList();
}

void MainWindow::addStandartPattern()
{
    PhObject obj;
    obj.setName("Стандартный");
    Patterns.push_front(obj);
    current_pattern = 0;
    ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    ui->deletepattern_button->setEnabled(true);
}

long double MainWindow::getDistanceOfCollision(int i, int j)
{
    return (Objects[i].getRadius()*10 + Objects[j].getRadius()*10) - getDistance(i, j);
}

void MainWindow::saveModel()
{
    if(!QFile::exists(Simulation_State.getFileWay()))
        saveModelAs();
    else
        save();
}

void MainWindow::saveModelAs()
{
    bool tmp_pause = isPause;
    setPause(true);
    QString way = QFileDialog::getSaveFileName(0, "Сохранить как...", "*.sps");
    Simulation_State.setFileWay(way);
    save();
    setPause(tmp_pause);
}

void MainWindow::openModel()
{
    if(hasModifided())
    {
        int res;

        bool tmp_pause = isPause;
        setPause(true);
        QMessageBox msgb;
        msgb.setWindowTitle("Отркытие файла");
        msgb.setText("Текущая модель была изменена");
        msgb.setInformativeText("Хотите сохранить её, прежде чем открыть?");
        msgb.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
        msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Question));
        msgb.setWindowIcon(windowIcon());
        res = msgb.exec();

        if(res == QMessageBox::Save)
            saveModel();

        if(res == QMessageBox::Cancel)
            return;

        setPause(tmp_pause);
    }

    QString prev_way = Simulation_State.getFileWay();
    QString way = QFileDialog::getOpenFileName(0, "Открыть файл", "*.sps");
    Simulation_State.setFileWay(way);
    open(prev_way);
    updateList();
    updateViewport();
    setConstFields();
}

void MainWindow::openDocumentation(QString str /*= "Основная информация о программе"*/)
{
    DocumentationWidget win(str);

    win.setWindowIcon(windowIcon());

    win.exec();
}

void MainWindow::openHello()
{
    HelloWindow *h_wind = new HelloWindow(version, this);

    h_wind->setWindowIcon(windowIcon());

    h_wind->exec();
}

//Создание новой модели
void MainWindow::newModel()
{
    Objects.clear();

    Save_Buffer_Objects = Objects;
    Buffer_Objects = Objects;

    Current_Collision_Mode = Programm_Settings.COLLISION_MODE;
    ui->viewport->setBackgroundColor(Programm_Settings.BACKGROUND_COLOR);
    this->G = Programm_Settings.G;
    this->k = Programm_Settings.K;

    BCamX = 0;
    BCamY = 0;
    delta = 0;
    changeFollowSetting(false);
    setPause(true);
    updateList();
    setFocus();
    updateViewport();

    G = Programm_Settings.G;
    k = Programm_Settings.K;

    ui->viewport->setCamX(0);
    ui->viewport->setCamY(0);
    updateCameraLabel();

    ui->statusBar->showMessage("Загружена новая модель");

    QString str = "/Новая модель";
    setNewWindowTitle(str);
    Simulation_State.setEmptyWay();
}

//Проверяет наличие изменений в текущей симуляции
bool MainWindow::hasModifided()
{
    return !vectorEquals(Objects, Save_Buffer_Objects);
}

bool MainWindow::vectorEquals(QVector<PhObject> &vec1, QVector<PhObject> &vec2)
{
    if(vec1.size() != vec2.size())
        return false;

    bool suc;

    for(int i = 0; i < vec1.size(); i++)
    {
        suc = vec1[i] != vec2[i];

        if(suc)
            return  false;
    }

    return true;
}

void MainWindow::followToObject(PhObject &obj)
{
    ui->viewport->setCamX(obj.getXPosition());
    ui->viewport->setCamY(obj.getYPosition());

    updateCameraLabel();
}

void MainWindow::clearObjectsVector()
{
    Objects.clear();

    updateList();
    updateViewport();
}

void MainWindow::loadSettings()
{
    QFile settings_file(settings_way);
    QTextStream reader(&settings_file);
    QString buffer;
    int pos = 0;

    QRegExp float_reg("\\-?\\d{1,}\\.?\\d{1,}e?\\-?\\d{1,}");
    float_reg.setPatternSyntax(QRegExp::RegExp);

    QRegExp int_reg("\\-?\\d{1,}");
    int_reg.setPatternSyntax(QRegExp::RegExp);

    bool isCorrect = true;

    int cm, bac_color;

    if(settings_file.exists())
    {
        if(settings_file.open(QIODevice::Text | QIODevice::ReadOnly))
        {
        reader.setRealNumberPrecision(20);

        QString line;

        reader >> buffer;
        Programm_Settings.SET_PAUSE_AFTER_CREATE = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.SET_PAUSE_AFTER_DELETE = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.SET_PAUSE_AFTER_RESTART = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.CAMERA_BUFFER_ENABLE = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        cm = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.G = buffer.toDouble();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.K = buffer.toDouble();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.SCALE_SPEED = buffer.toDouble();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        bac_color = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.RENDER_MODE = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.SIMULATION_SPEED = buffer.toDouble();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        reader >> buffer;
        Programm_Settings.OPEN_FULLSCREEN = buffer.toInt();
        isCorrect &= float_reg.exactMatch(buffer) || int_reg.exactMatch(buffer);

        switch (cm)
        {
        case 0:
            Programm_Settings.COLLISION_MODE = CollisionMode::MERGE;
            break;
        case 1:
            Programm_Settings.COLLISION_MODE = CollisionMode::ELASTIC;
            break;
        case 2:
            Programm_Settings.COLLISION_MODE = CollisionMode::NOT_ELASTIC;
            break;

        default:
            Programm_Settings.COLLISION_MODE = CollisionMode::MERGE;
            break;
        }

        switch (bac_color)
        {
        case 0:
            Programm_Settings.BACKGROUND_COLOR = Qt::white;
            break;
        case 1:
            Programm_Settings.BACKGROUND_COLOR = Qt::blue;
            break;
        case 2:
            Programm_Settings.BACKGROUND_COLOR = Qt::darkGray;
            break;
        case 3:
            Programm_Settings.BACKGROUND_COLOR = Qt::darkRed;
            break;
        case 4:
            Programm_Settings.BACKGROUND_COLOR = Qt::darkGreen;
            break;
        case 5:
            Programm_Settings.BACKGROUND_COLOR = Qt::darkBlue;
            break;
        case 6:
            Programm_Settings.BACKGROUND_COLOR = Qt::darkYellow;
            break;

        default:
            Programm_Settings.BACKGROUND_COLOR = Qt::white;
            break;
        }

        settings_file.close();

        if(!isCorrect)
        {
            QMessageBox msgb;
            msgb.setWindowTitle("Предупреждение");
            msgb.setText("Файл настроек был повреждён");
            msgb.setInformativeText("Настройки будут установлены по умолчанию");
            msgb.setIcon(QMessageBox::Icon(QMessageBox::Icon::Information));
            msgb.setWindowIcon(windowIcon());

            msgb.exec();

            Programm_Settings.SetDefaultSettings();
            saveSettings();
        }

        }

        else
        {
            QMessageBox::warning(this, "Ошибка", "Не доступа к файлу настроек, программа может работать некорректно (Настройки будут установлены по умолчанию)");
            Programm_Settings.SetDefaultSettings();
        }
    }

    else
    {
        Programm_Settings.SetDefaultSettings();
        saveSettings();
    }
}

void MainWindow::saveSettings()
{
    QFile settings_file(settings_way);

    QTextStream writer(&settings_file);

    QDir dir(QCoreApplication::applicationDirPath());

    if(!dir.exists("Settings"))
        dir.mkdir("Settings");

    if(settings_file.open(QIODevice::Text | QIODevice::WriteOnly))
    {
       writer.setRealNumberPrecision(20);

       writer << (int)Programm_Settings.SET_PAUSE_AFTER_CREATE << endl;
       writer << (int)Programm_Settings.SET_PAUSE_AFTER_DELETE << endl;
       writer << (int)Programm_Settings.SET_PAUSE_AFTER_RESTART << endl;
       writer << (int)Programm_Settings.CAMERA_BUFFER_ENABLE << endl;

       if(Programm_Settings.COLLISION_MODE == CollisionMode::MERGE)
           writer << 0 << endl;
       if(Programm_Settings.COLLISION_MODE == CollisionMode::ELASTIC)
           writer << 1 << endl;
       if(Programm_Settings.COLLISION_MODE == CollisionMode::NOT_ELASTIC)
           writer << 2 << endl;

       writer << Programm_Settings.G << endl;
       writer << Programm_Settings.K << endl;
       writer << Programm_Settings.SCALE_SPEED << endl;

       {
           if(Programm_Settings.BACKGROUND_COLOR == Qt::white)
               writer << 0 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::cyan)
               writer << 1 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::darkGray)
               writer << 2 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::darkRed)
               writer << 3 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::darkGreen)
               writer << 4 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::darkBlue)
               writer << 5 << endl;
           if(Programm_Settings.BACKGROUND_COLOR == Qt::darkYellow)
               writer << 6 << endl;
       }

       writer << (int)Programm_Settings.RENDER_MODE << endl;
       writer << Programm_Settings.SIMULATION_SPEED << endl;
       writer << (int)Programm_Settings.OPEN_FULLSCREEN << endl;

       settings_file.close();
    }
}

int getRandom(int p1, int p2)
{
    return p1 + rand() % (p2 - p1 + 1);
}

void MainWindow::randomGenerate(GeneratePattern &pattern, int count)
{
    PhObject obj;
    double side_size = sqrt(count * pattern.free_space);
    long double angle;

    for(int i = 0; i < count; i++)
    {
        obj.setName(pattern.name + " " + QString::number(i));
        obj.setMass((double)getRandom(pattern.m1*100000, pattern.m2*100000)/100000);
        obj.setQ((double)getRandom(pattern.q1*100000, pattern.q2*100000)/100000);

        if(pattern.rad_auto)
        {
            obj.setRadius(obj.getMass() * pattern.p);
            if(obj.getRadius() < 0.1)
                obj.setRadius(0.1);
        }

        else
            obj.setRadius((double)getRandom(pattern.rad1*10, pattern.rad2*10) / 10);


        obj.setXPosition(getRandom(-side_size/2, side_size/2) + pattern.x0);
        obj.setYPosition(getRandom(-side_size/2, side_size/2) + pattern.y0);
        angle = std::atan2(obj.getYPosition() - pattern.y0, obj.getXPosition() - pattern.x0);
        obj.setXSpeed(std::cos(angle) * pattern.start_speed + getRandom(-pattern.max_speed*1000,
            pattern.max_speed*1000)/1000 + std::cos(1.570796326 + angle) * pattern.tangentum_speed * getDistance(pattern.x0, pattern.y0, obj) / side_size);

        obj.setYSpeed(std::sin(angle) * pattern.start_speed + getRandom(-pattern.max_speed*1000,
            pattern.max_speed*1000)/1000 + std::sin(1.570796326 + angle) * pattern.tangentum_speed * getDistance(pattern.x0, pattern.y0, obj) / side_size);

        obj.setColor(pattern.colors[getRandom(0, pattern.colors.size() - 1)]);

        Objects.push_back(obj);
    }

    updateList();
    updateViewport();
    setPause(true);
}

bool MainWindow::checkIndexValid(int index, QVector<PhObject> &vec)
{
    return index >= 0 && index < vec.size();
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
    if(Objects[current_index].getStatic())
        Objects[current_index].setSpeed(0, 0);

    updateViewport();
    }

    else
    {
    Patterns[current_pattern].setMass(ui->mass_line->text().toDouble() * pow(10, getPowerMnog(ui->mass_mnog)));
    Patterns[current_pattern].setQ(ui->q_line->text().toDouble() * pow(10, getPowerMnog(ui->q_mnog)));
    Patterns[current_pattern].setRadius(ui->rad_line->text().toDouble());
    Patterns[current_pattern].setStatic(ui->isMoveBox->isChecked());
    Patterns[current_pattern].setColor(getColorBox());

    savePatterns();
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
    obj.setName("Новый шаблон " + QString::number(Patterns.size() + 1));
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
    if(checkIndexValid(current_pattern, Patterns))
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

    setPatternFocus(current_pattern);

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

    savePatterns();
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
    setPatternFocus(currentRow);
}

void MainWindow::setPatternFocus(int index)
{
    if(checkIndexValid(index, Patterns))
    {
    setPatternMode(true);
    current_pattern = index;
    ui->Propertites_box->setEnabled(true);
    ui->choosen_label->setText("Выбран: " + Patterns[current_pattern].getName());
    printToPanel(Patterns[index]);
    }

    else
    {
        ui->choosen_label->setText("Шаблонов нет");
        setFocus();
    }
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

void MainWindow::changeScaleSlot(double value)
{
    ui->scale_slider->setValue(value);
    ui->scale_label->setText(QString::number(value) + "%");
    ui->viewport->setScale((double)value / 100);
}

void MainWindow::changeScaleSlot(int value)
{
    changeScaleSlot((double)value);
}

void MainWindow::updateCameraLabel()
{
    ui->cam_pos_label->setText("Камера [" + QString::number(ui->viewport->getCamX()) + "; " + QString::number(ui->viewport->getCamY()) + "]");
}

void MainWindow::moveCameraToCenter()
{
    ui->viewport->setCamPos();
    updateCameraLabel();
    ui->statusBar->showMessage("Координаты камеры установлены на [0; 0]");
}

void MainWindow::dropCameraScale()
{
    changeScaleSlot(100);
    ui->statusBar->showMessage("Масштаб установлен на 100%");
}

void MainWindow::setBuffer()
{
    BCamX = ui->viewport->getCamX();
    BCamY = ui->viewport->getCamY();
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
        followToObject(Objects[current_index]);
        ui->viewport->update();
    }

}

void MainWindow::changeFollowSetting()
{
    follow_to_focus_object = !follow_to_focus_object;

    if(follow_to_focus_object)
    {
        ui->follow_button->setIcon(QIcon(QPixmap(":/img/not_follow.png")));
        ui->follow_button->setToolTip("Не следить за выбранным объектом CTRL+CAPSLOCK");
    }

    else
    {
        ui->follow_button->setIcon(QIcon(QPixmap(":/img/follow.png")));
        ui->follow_button->setToolTip("Следить за выбранным объектом CTRL+CAPSLOCK");
    }
}

void MainWindow::changeFollowSetting(bool val)
{
    follow_to_focus_object = val;

    if(follow_to_focus_object)
    {
        ui->follow_button->setIcon(QIcon(QPixmap(":/img/not_follow.png")));
        ui->follow_button->setToolTip("Не следить за выбранным объектом CTRL+CAPSLOCK");
    }

    else
    {
        ui->follow_button->setIcon(QIcon(QPixmap(":/img/follow.png")));
        ui->follow_button->setToolTip("Следить за выбранным объектом CTRL+CAPSLOCK");
    }
}

void MainWindow::makeCenter()
{
    for(auto &var : Objects)
    {
        var.setPosition(var.getXPosition() - ui->viewport->getCamX(), var.getYPosition() - ui->viewport->getCamY());
    }

    for(auto &var : Buffer_Objects)
    {
        var.setPosition(var.getXPosition() - ui->viewport->getCamX(), var.getYPosition() - ui->viewport->getCamY());
    }

    BCamX -= ui->viewport->getCamX();
    BCamY -= ui->viewport->getCamY();

    ui->viewport->setCamPos();
    updateCameraLabel();
    ui->statusBar->showMessage("Начало координат установлено на положение камеры");

    if(checkIndexValid(current_index, Objects))
        printToPanel(Objects[current_index]);

    updateViewport();
}

void MainWindow::makeNullSpeed()
{
    if(checkIndexValid(current_index, Objects))
    {
        long double dx = Objects[current_index].getXSpeed();
        long double dy = Objects[current_index].getYSpeed();

        for(auto &var : Objects)
        {
            var.setSpeed(var.getXSpeed() - dx, var.getYSpeed() - dy);
        }

        ui->statusBar->showMessage("Мировая скорость равняется скорости объекта: " + Objects[current_index].getName());

        if(checkIndexValid(current_index, Objects))
            printToPanel(Objects[current_index]);

        updateViewport();
    }

    else
    {
        ui->statusBar->showMessage("Невозможно установить новую мировую скорость, так как ни один объект не выбран!");
    }


}

void MainWindow::OpenGenerateWidget()
{
    GeneratePattern pat;
    pat.x0 = ui->viewport->getCamX();
    pat.y0 = ui->viewport->getCamY();
    bool suc;
    int count;

    GenerateWidget* win = new GenerateWidget(Patterns, this, &suc, &pat, &count);
    win->exec();

    if(suc)
        randomGenerate(pat, count);
}



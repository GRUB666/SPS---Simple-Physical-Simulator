#include "mainwindow.h"
#include <QApplication>


//Location: C:\Qt\Projects\SimplePhysicalSimulator
//Github rep: https://github.com/GRUB666/SPS---Simple-Physical-Simulator.git


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    return a.exec();
}

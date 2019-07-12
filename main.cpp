#include "mainwindow.h"
#include <QApplication>


//Location: C:\Qt\Projects\SimplePhysicalSimulator\SPS---Simple-Physical-Simulator
//Github rep: https://github.com/GRUB666/SPS---Simple-Physical-Simulator.git


int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SKY computers");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("Simple Physical Simulator");

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/img/Mainico.ico"));

    MainWindow w("1.1");
    w.showMaximized();

    return a.exec();
}

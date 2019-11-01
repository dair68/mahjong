#include "mainwindow.h"
#include "shisenwidget.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //title screen
//    MainWindow w;
//    w.show();

    //shisensho screen
    QWidget* shisenWindow = new ShisenWidget(nullptr, 5, 4);
    shisenWindow->show();

    return a.exec();
}

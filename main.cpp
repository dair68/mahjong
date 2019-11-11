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
    ShisenWidget shisenWin;
    shisenWin.show();

    return a.exec();
}

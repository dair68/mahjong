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
//    QWidget* shisen = new ShisenWidget();
//    MainWindow w(shisen);
//    w.show();

    QWidget* shisenWindow = new ShisenWidget();
    shisenWindow->show();

    return a.exec();
}

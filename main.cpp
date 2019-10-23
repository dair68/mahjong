#include "mainwindow.h"
#include "shisensho.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    //shisensho screen
    QWidget* shisen = new Shisensho();
    MainWindow w(shisen);

    w.show();
    return a.exec();
}

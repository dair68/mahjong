#include "mainwindow.h"
#include "shisenwidget.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setting seed for random tiles
    srand(time(0));

    //title screen
//    MainWindow w;
//    w.show();

    //shisensho screen
    ShisenWidget shisenWin;
    shisenWin.show();
    shisenWin.startGame();

    //generating and saving random shisensho games
//    generateShisenshoGames(10, "small_levels.txt", "small");

    return a.exec();
}

#include "mainwindow.h"
#include "shisenwidget.h"
#include "helpwidget.h"
#include "titlescreen.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <iostream>
#include "stopwatch.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setting seed for random tiles
    srand(time(0));

    //main window
    MainWindow w;
    w.show();

    //title screen
//    TitleScreen title;
//    title.show();

    //stopwatch
//    Stopwatch time;
//    time.increaseTime(3500);
//    time.show();
//    time.start();

    //shisensho screen
//    ShisenWidget shisenWin;
//    shisenWin.show();
//    shisenWin.startGame();

    //help widget
//    HelpWidget help;
//    help.show();

    //generating and saving random shisensho games
//    generateShisenshoGames(50, "small_levels.txt", "small");
//    qDebug() << "done making levels";

    //generating medium levels
//    generateShisenshoGames(1, "medium_levels.txt", "medium");
//    qDebug() << "done making levels";

    //generating large levels
//    generateShisenshoGames(2, "large_levels.txt", "large");
//    qDebug() << "done making levels";

    return a.exec();
}

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
//    ShisenWidget shisenWin;
//    shisenWin.show();
//    shisenWin.startGame();

    //generating and saving random shisensho games
    generateShisenshoGames(10, "small_levels.txt", "small");

//    QFile file("out.txt");
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)){
//        qDebug() << "cant open file";
//        return 0;
//    }


//    QTextStream out(&file);
//    QString line = "The magic number is: " + QString::number(49) + "\n";
//    out << line;
//    qDebug() << "done writing";
//    file.close();

    return a.exec();
}

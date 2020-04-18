#include "mainwindow.h"
#include "titlescreen.h"
#include "shisenwidget.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* title = new TitleScreen(this);
    setCentralWidget(title);
    initializeSettings();
}

MainWindow::MainWindow(QWidget* central, QWidget* parent) : QMainWindow(parent) {
    QMainWindow::setCentralWidget(central);
    initializeSettings();
}

void MainWindow::initializeSettings() {
    QString tilePath = ":/images/MJt1-.svg.png";
    setWindowIcon(QIcon(tilePath));

    int scale = 300;
    int width = 4 * scale;
    int height = 3 * scale;
    this->setFixedSize(width, height);
}

void MainWindow::toShisensho() {
    ShisenWidget* shisen = new ShisenWidget(this);
    setCentralWidget(shisen);
    shisen->startGame();
}




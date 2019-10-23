#include "mainwindow.h"
#include "titlescreen.h"
#include "shisensho.h"
#include <iostream>

MainWindow::MainWindow() {
    QWidget* title = new TitleScreen(this);
    QMainWindow::setCentralWidget(title);
    initializeSettings();
}

MainWindow::MainWindow(QWidget* central) {
    QMainWindow::setCentralWidget(central);
    initializeSettings();
}

void MainWindow::initializeSettings() {
    setWindowIcon(QIcon(":/images/icon.png"));

    int scale = 300;
    int width = 4 * scale;
    int height = 3 * scale;
    this->setFixedSize(width, height);
}

void MainWindow::toShisensho() {
    QWidget* shisen = new Shisensho(this);
    this->setCentralWidget(shisen);
}




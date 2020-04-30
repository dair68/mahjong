#include "mainwindow.h"
#include "titlescreen.h"
#include "shisenwidget.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), help() {
    QString tilePath = ":/images/MJt1-.svg.png";
    setWindowIcon(QIcon(tilePath));

    int scale = 300;
    int width = 4 * scale;
    int height = 3 * scale;
    setFixedSize(width, height);

    toTitle();
}

void MainWindow::toShisensho() {
    ShisenWidget* shisen = new ShisenWidget(this);
    setCentralWidget(shisen);
    connect(shisen, &ShisenWidget::returnToTitle, this, &MainWindow::toTitle);
    connect(shisen, &ShisenWidget::showHelp, &help, &QWidget::show);
    shisen->startNewGame();
}

void MainWindow::toTitle() {
    TitleScreen* title = new TitleScreen(this);
    setCentralWidget(title);

    connect(title, &TitleScreen::playButtonClicked, this, &MainWindow::toShisensho);
    connect(title, &TitleScreen::tutorialButtonClicked, &help, &QWidget::show);

    emit enteredTitleScreen();
}

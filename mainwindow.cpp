#include "mainwindow.h"
#include "titlescreen.h"

MainWindow::MainWindow() {
    QWidget* title = new TitleScreen();
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




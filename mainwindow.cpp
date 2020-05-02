#include "mainwindow.h"
#include "titlescreen.h"
#include "shisenwidget.h"
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlaylist>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), help(), player() {
    QString tilePath = ":/images/MJt1-.svg.png";
    setWindowIcon(QIcon(tilePath));

    int scale = 300;
    int width = 4 * scale;
    int height = 3 * scale;
    setFixedSize(width, height);

    connect(this, &MainWindow::enteredTitleScreen, &player, &MusicPlayer::playTitleTheme);
    connect(this, &MainWindow::enteredShisenScreen, &player, &MusicPlayer::pause);

    toTitle();
}

void MainWindow::toShisensho() {
    ShisenWidget* shisen = new ShisenWidget(this);
    setCentralWidget(shisen);
    emit enteredShisenScreen();

    connect(shisen, &ShisenWidget::returnToTitle, this, &MainWindow::toTitle);
    connect(shisen, &ShisenWidget::showHelp, &help, &QWidget::show);
    connect(shisen, &ShisenWidget::gameHasBegun, &player, &MusicPlayer::playGameTheme);
    connect(shisen, &ShisenWidget::gamePaused, &player, &MusicPlayer::pause);
    connect(shisen, &ShisenWidget::gameResumed, &player, &MusicPlayer::resume);
    connect(shisen, &ShisenWidget::gameOver, &player, &MusicPlayer::pause);
    shisen->startNewGame();
}

void MainWindow::toTitle() {
    TitleScreen* title = new TitleScreen(this);
    setCentralWidget(title);
    emit enteredTitleScreen();

    connect(title, &TitleScreen::playButtonClicked, this, &MainWindow::toShisensho);
    connect(title, &TitleScreen::tutorialButtonClicked, &help, &QWidget::show);
}

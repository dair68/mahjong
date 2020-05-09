#include "mainwindow.h"
#include "titlewidget.h"
#include "shisenwidget.h"
#include "creditswidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), help(), player() {
    const QString tilePath = ":/images/MJt1-.svg.png";
    setWindowIcon(QIcon(tilePath));

    const int scale = 300;
    const int width = 4 * scale;
    const int height = 3 * scale;
    setFixedSize(width, height);

    connect(this, SIGNAL(enteredShisenScreen()), &player, SLOT(pause()));
    player.playTitleTheme();
    toTitle();
}

void MainWindow::toShisensho() {
    ShisenWidget* shisen = new ShisenWidget(this);
    setCentralWidget(shisen);
    emit enteredShisenScreen();

    connect(shisen, SIGNAL(returnToTitle()), this, SLOT(toTitle()));
    connect(shisen, SIGNAL(returnToTitle()), &player, SLOT(playTitleTheme()));
    connect(shisen, SIGNAL(showHelp()), &help, SLOT(show()));
    connect(shisen, SIGNAL(gameHasBegun()), &player, SLOT(playGameTheme()));
    connect(shisen, SIGNAL(gamePaused()), &player, SLOT(pause()));
    connect(shisen, SIGNAL(gameResumed()), &player, SLOT(resume()));
    connect(shisen, SIGNAL(gameOver()), &player, SLOT(pause()));

    shisen->startNewGame();
}

void MainWindow::toTitle() {
    TitleWidget* title = new TitleWidget(this);
    setCentralWidget(title);

    connect(title, SIGNAL(playButtonClicked()), this, SLOT(toShisensho()));
    connect(title, SIGNAL(tutorialButtonClicked()), &help, SLOT(show()));
    connect(title, SIGNAL(creditsButtonClicked()), this, SLOT(toCredits()));
}

void MainWindow::toCredits() {
    CreditsWidget* credits = new CreditsWidget(this);
    setCentralWidget(credits);
    connect(credits, SIGNAL(backButtonClicked()), this, SLOT(toTitle()));
}

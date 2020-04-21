#include "titlescreen.h"
#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFontDatabase>

TitleScreen::TitleScreen(MainWindow* parent) : QWidget(parent) {
    setObjectName("TitleScreen");
    setStyleSheet("QWidget#TitleScreen {background-image: url(:/images/chineseclouds-01.jpg);}");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);

    QLabel* title = new QLabel("Mahjong", this);
    QFontDatabase::addApplicationFont(":/fonts/ragingredlotus-bb.regular.ttf");

    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-family: RagingRedLotus BB;"
        "font-size: 130pt;"
        "font-style: italic;"
    );
    layout->addWidget(title);

    QPushButton* shisenButton = new QPushButton("Play",this);
    connect(shisenButton, SIGNAL(clicked()), parent, SLOT(toShisensho()));
    shisenButton->setStyleSheet(
        "font-size: 22pt;"
    );

    QPushButton* tutorialButton = new QPushButton("How To Play", this);
    connect(tutorialButton, SIGNAL(clicked()), parent, SLOT(showHelpDialog()));
    tutorialButton->setStyleSheet(
        "font-size: 22pt;"
    );

    layout->addWidget(shisenButton);
    layout->addWidget(tutorialButton);
    layout->addSpacing(200);

    this->setLayout(layout);
}

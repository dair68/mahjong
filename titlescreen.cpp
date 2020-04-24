#include "titlescreen.h"
#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFontDatabase>
#include <QStyleOption>
#include <QPainter>

TitleScreen::TitleScreen(QWidget* parent) : QWidget(parent) {
    QLabel* title = new QLabel("Mahjong", this);
    QFontDatabase::addApplicationFont(":/fonts/ragingredlotus-bb.regular.ttf");

    title->setAlignment(Qt::AlignCenter);
    setStyleSheet(
                "QLabel {"
                "font-family: RagingRedLotus BB;"
                "font-size: 130pt;"
                "font-style: italic;"
                "}"
                "QPushButton {"
                "font-size: 22pt;"
                "}"
                "TitleScreen {"
                "background-image: url(:/images/chineseclouds-01.jpg);"
                "}"
                );

    QPushButton* shisenButton = new QPushButton("Play",this);
    connect(shisenButton, &QPushButton::clicked, this, &TitleScreen::playButtonClicked);

    QPushButton* tutorialButton = new QPushButton("How To Play", this);
    connect(tutorialButton, &QPushButton::clicked, this, &TitleScreen::tutorialButtonClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);

    layout->addWidget(title);
    layout->addWidget(shisenButton);
    layout->addWidget(tutorialButton);
    layout->addSpacing(200);
    setLayout(layout);
}

void TitleScreen::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#include "titlescreen.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFontDatabase>

TitleScreen::TitleScreen(QWidget* parent) : QWidget(parent) {
    setObjectName("TitleScreen");
    setStyleSheet("QWidget#TitleScreen {background-image: url(:/images/chineseclouds-01.jpg);}");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);

    QLabel* title = new QLabel("Mahjong", this);
    QFontDatabase::addApplicationFont(":/fonts/ragingredlotus-bb.regular.ttf");

    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-family: RagingRedLotus BB;"
        "font-size: 150pt;"
        "font-style: italic;"
    );
    layout->addWidget(title);
    //layout->addSpacing(80);

    QPushButton* shisenButton = new QPushButton("Shisen-sho",this);
    shisenButton->setStyleSheet(
        "font-size: 25pt;"
    );
    layout->addWidget(shisenButton);
    layout->addSpacing(150);

    this->setLayout(layout);
}

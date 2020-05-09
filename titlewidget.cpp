#include "titlewidget.h"
#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFontDatabase>
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include <QApplication>

TitleWidget::TitleWidget(QWidget* parent) : QWidget(parent) {
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
                "TitleWidget {"
                "background-image: url(:/images/chineseclouds-01.jpg);"
                "}"
                );

    QPushButton* shisenButton = new QPushButton("Play",this);
    connect(shisenButton, SIGNAL(clicked()), this, SIGNAL(playButtonClicked()));

    QPushButton* tutorialButton = new QPushButton("How To Play", this);
    connect(tutorialButton, SIGNAL(clicked()), this, SIGNAL(tutorialButtonClicked()));

    QPushButton* creditsButton = new QPushButton("Credits", this);
    connect(creditsButton, SIGNAL(clicked()), this, SIGNAL(creditsButtonClicked()));

    QPushButton* quitButton = new QPushButton("Quit", this);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(showQuitDialog()));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);

    layout->addWidget(title);
    layout->addWidget(shisenButton);
    layout->addWidget(tutorialButton);
    layout->addWidget(creditsButton);
    layout->addWidget(quitButton);
    layout->addSpacing(100);
    setLayout(layout);
}

void TitleWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleWidget::showQuitDialog() {
    QMessageBox dialog;
    dialog.setText("Are you sure you want to quit?");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = dialog.exec();

    //quitting if applicable
    switch (ret) {
        case QMessageBox::Yes:
            QApplication::quit();
            break;
        default:
            break;
    }
}

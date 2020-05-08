#include "helpwidget.h"
#include <QLabel>
#include <QVBoxLayout>

HelpWidget::HelpWidget(QWidget* parent) : QTabWidget(parent) {
    resize(800, 600);
    setStyleSheet("font-size: 12pt");

    createOverviewTab();
    createBasicsTab();
    createRemoveTab();
    createSpecialTab();
    createToolsTab();

   // setAttribute( Qt::WA_DeleteOnClose, true );
}

void HelpWidget::createOverviewTab() {
    //creating widget
    QWidget* overview = new QWidget(this);
    QLabel* textLabel = new QLabel(overview);
    textLabel->setText(
                "This program features a single player mahjong game called "
                "Shisensho. The goal is to get rid of all the tiles as fast "
                "as possible. The game ends when all the tiles are removed "
                "or when there are no more removable tiles."
                );
    textLabel->setWordWrap(true);

    QLabel* pictureLabel = new QLabel(overview);
    const QString picturePath = ":/images/screenshot_1.png";
    pictureLabel->setPixmap(QPixmap(picturePath));

    //creating layout
    QVBoxLayout* overviewLayout = new QVBoxLayout(overview);
    overviewLayout->addWidget(textLabel);
    overviewLayout->addWidget(pictureLabel);
    overviewLayout->setAlignment(pictureLabel, Qt::AlignHCenter);

    overview->setLayout(overviewLayout);
    addTab(overview, "Overview");
}

void HelpWidget::createBasicsTab() {
    //creating widget
    QWidget* basics = new QWidget(this);
    const QString text1 = "Click on a tile to select it. Selected tiles have a "
                          "bright shine. Click that tile again to deselect it. "
                          "Once two tiles have been selected, one of the following will happen: ";
    const QString text2 = "1. The tiles are nonremovable and are automatically "
                          "deselected alongside a five second time penalty.";
    const QString text3 = "2. The tiles can be removed. A path is drawn between them and"
                          " they disappear.";

    const std::vector<QString> textStrings = {text1, text2, text3};
    const std::vector<QString> imageNames = {"selected_pic", "mismatch_tiles", "matching_tiles"};

    QGridLayout* layout = new QGridLayout(basics);
    layout->setColumnStretch(1,1);

    //creating widgets and adding them to layout
    for(int i=0; i<textStrings.size(); i++) {
        QLabel* pic = new QLabel(basics);
        const QString imagePath = ":/images/" + imageNames[i] + ".png";
        pic->setPixmap(QPixmap(imagePath));
        layout->addWidget(pic, i, 0);
        layout->setAlignment(pic, Qt::AlignHCenter);

        QLabel* text = new QLabel(textStrings[i], basics);
        text->setWordWrap(true);
        layout->addWidget(text, i, 1);
    }

    basics->setLayout(layout);
    addTab(basics, "Basics");
}

void HelpWidget::createRemoveTab() {
    //creating widget
    QWidget* remove = new QWidget(this);
    QLabel* label = new QLabel(remove);
    const QString text = "A pair of tiles can be removed if they have matching symbols and"
                   " can be connected by an uninterrupted path with no more than 2 bends.";
    label->setText(text);
    label->setWordWrap(true);

    QGridLayout* layout = new QGridLayout(remove);
    layout->addWidget(label, 0, 0, 1, 3);

    //adding screenshots
    for(int i=0; i < 6; i++) {
        QLabel* pic = new QLabel(remove);
        const QString imagePath = ":/images/match_" + QString::number(i + 1) + ".png";
        pic->setPixmap(QPixmap(imagePath));
        layout->addWidget(pic, i/3 + 1, i % 3);
    }

    remove->setLayout(layout);
    addTab(remove, "Removing Tiles");
}

void HelpWidget::createSpecialTab() {
    //creating widget
    QWidget* special = new QWidget(this);
    QLabel* label = new QLabel(special);
    label->setText(
                "Any pair of season tiles (blue number in upper left) can be removed "
                "so long as they can be connected by an uninterrupted path with "
                "no more than 2 bends. Likewise, any pair of "
                "flower tiles (red number in upper right) can be removed so long as "
                "they satisfy the path requirement."
                );
    label->setWordWrap(true);

    QLabel* pic1 = new QLabel(special);
    const QString path1 = ":/images/special_1.png";
    pic1->setPixmap(QPixmap(path1));

    QLabel* pic2 = new QLabel(special);
    const QString path2 = ":/images/special_2.png";
    pic2->setPixmap(QPixmap(path2));

    //creating layout
    QGridLayout* layout = new QGridLayout(special);
    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(pic1, 1, 0);
    layout->setAlignment(pic1, Qt::AlignHCenter);
    layout->addWidget(pic2, 1, 1);
    layout->setAlignment(pic2, Qt::AlignHCenter);

    special->setLayout(layout);
    addTab(special, "Special Tiles");
}

void HelpWidget::createToolsTab() {
    //creating widget
    QWidget* tools = new QWidget(this);
    QLabel* textLabel1 = new QLabel(tools);
    textLabel1->setText(
                "Click the \'Undo\' button to restore the most recently removed "
                "pair of tiles. A new pair of tiles must be removed in order to use "
                "undo again."
                );
    textLabel1->setWordWrap(true);

    QLabel* pic1 = new QLabel(tools);
    const QString path1 = ":/images/undo.png";
    pic1->setPixmap(QPixmap(path1));

    QLabel* textLabel2 = new QLabel(tools);
    textLabel2->setText(
                "Click the \'Hint\' button to highlight all currently removable "
                "tiles. Each click of this button results in a 60 second time penalty."
                );
    textLabel2->setWordWrap(true);

    QLabel* pic2 = new QLabel(tools);
    const QString path2 = ":/images/hint.png";
    pic2->setPixmap(QPixmap(path2));

    //creating layout
    QVBoxLayout* layout = new QVBoxLayout(tools);
    layout->addWidget(textLabel1);
    layout->addWidget(pic1);
    layout->setAlignment(pic1, Qt::AlignHCenter);
    layout->addWidget(textLabel2);
    layout->addWidget(pic2);
    layout->setAlignment(pic2, Qt::AlignHCenter);

    tools->setLayout(layout);
    addTab(tools, "Help Buttons");
}

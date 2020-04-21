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
}

void HelpWidget::createOverviewTab() {
    //creating widget
    QWidget* overview = new QWidget();
    QString text = "This program features a single player mahjong game called "
                   "Shisensho. The goal is to get rid of all the tiles as fast "
                   "as possible. The game ends when all the tiles are removed "
                   "or when there are no more removable tiles.";
    QLabel* overviewLabel = new QLabel();
    overviewLabel->setText(text);
    overviewLabel->setWordWrap(true);

    //creating layout
    QVBoxLayout* overviewLayout = new QVBoxLayout();
    overviewLayout->addWidget(overviewLabel);
    overview->setLayout(overviewLayout);
    addTab(overview, "Overview");
}

void HelpWidget::createBasicsTab() {
    //creating widget
    QWidget* basics = new QWidget();
    QLabel* basicsLabel = new QLabel();
    QString basicsText = "Click on a tile to select it. Selected tiles have a green "
                         "hue. Click that tile again to deselect it.\n\nOnce two tiles"
                         " have been selected, one of the following will happen:\n\n1. "
                         "The tiles are nonremovable and are automatically "
                         "deselected alongside a 5 second time penalty.\n\n"
                         "2. The tiles can be removed. A path is drawn between them and"
                         " they disappear.";
    basicsLabel->setText(basicsText);
    basicsLabel->setWordWrap(true);

    //creating layout
    QVBoxLayout* basicsLayout = new QVBoxLayout();
    basicsLayout->addWidget(basicsLabel);
    basics->setLayout(basicsLayout);
    addTab(basics, "Basics");
}

void HelpWidget::createRemoveTab() {
    //creating widget
    QWidget* remove = new QWidget();
    QLabel* label = new QLabel();
    QString text = "A pair of tiles can be removed if they have matching symbols and"
                   " can be connected by an uninterrupted path with no more than 2 bends.";
    label->setText(text);
    label->setWordWrap(true);

    //creating layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    remove->setLayout(layout);
    addTab(remove, "Removing Tiles");
}

void HelpWidget::createSpecialTab() {
    //creating widget
    QWidget* special = new QWidget();
    QLabel* label = new QLabel();
    QString text = "Any pair of season tiles (blue number in upper left) can be removed "
                   "so long as they can be connected by an uninterrupted path with "
                   "no more than 2 bends. Likewise, any pair of "
                   "flower tiles (red number in upper right) can be removed so long as "
                   "they satisfy the path requirement.";
    label->setText(text);
    label->setWordWrap(true);

    //creating layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    special->setLayout(layout);
    addTab(special, "Special Tiles");
}

void HelpWidget::createToolsTab() {
    //creating widget
    QWidget* tools = new QWidget();
    QString text = "Click the \'Undo\' button to restore the most recently removed "
                   "pair of tiles. A new pair of tiles must be removed in order to use "
                   "undo again.\n\nClick the \'Hint\' button to highlight all currently removable "
                   "tiles. Each click of this button results in a 60 second time penalty.";
    QLabel* label = new QLabel(text);
    label->setWordWrap(true);

    //creating layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    tools->setLayout(layout);
    addTab(tools, "Help Buttons");
}

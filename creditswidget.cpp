#include "creditswidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

CreditsWidget::CreditsWidget(QWidget* parent) : QWidget(parent) {
    const int scale = 300;
    const int width = 4 * scale;
    const int height = 3 * scale;
    setMinimumSize(width, height);

    QPushButton* backButton = new QPushButton("Back to title", this);
    backButton->move(width - 250, height - 100);
    backButton->resize(190, 80);
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(backButtonClicked()));

    createCreditLabels();
    initializeStyleSheet();
}

void CreditsWidget::paintEvent(QPaintEvent *event) {
    QStyleOption option;
    option.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
    QWidget::paintEvent(event);
}

void CreditsWidget::createCreditLabels() {
    QLabel* title = new QLabel("<h1>Credits</h1>", this);
    QLabel* imageCredits = createImageCredits();
    QLabel* musicCredits = createMusicCredits();
    QLabel* soundCredits = createSoundCredits();
    QLabel* programming = createProgrammingCredits();

    const int spacing = 10;

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);
    layout->addWidget(title);
    layout->addWidget(imageCredits);
    layout->addSpacing(spacing);
    layout->addWidget(musicCredits);
    layout->addSpacing(spacing);
    layout->addWidget(soundCredits);
    layout->addSpacing(spacing);
    layout->addWidget(programming);
    setLayout(layout);
}

QLabel* CreditsWidget::createImageCredits() {
    QLabel* imageCredits = new QLabel(this);
    imageCredits->setText("<h2>Images</h2>"
                          "<p><a href=\"https://www.vecteezy.com/vector-art/105158-chinese-clouds-landscape-illustration-vector\"style=\"color: yellow;\">"
                          "<em>Chinese Clouds Landscape Illustration Vector</em></a> and "
                          "<a href=\"https://www.vecteezy.com/vector-art/82928-chinese-temple-landscape-vector-background\" style=\"color: yellow;\">"
                          "<em>Chinese Temple Landscape Vector Background</em></a> found from "
                          "<a href=\"http://vecteezy.com\" style=\"color: yellow;\">vecteezy.com</a></p>"
                          "<p>All <a href=\"https://commons.wikimedia.org/wiki/Category:SVG_Oblique_illustrations_of_Mahjong_tiles\" style=\"color: yellow;\">Mahjong</a> "
                          "<a href=\"https://commons.wikimedia.org/w/index.php?title=Category:SVG_Oblique_illustrations_of_Mahjong_tiles&filefrom=MJs4-.svg#mw-category-media\" style=\"color: yellow;\">"
                          "tiles</a> originally illustrated by Cangjie6. "
                          "Tiles recolored for the purpose of this application and used under "
                          "<a href=\"https://creativecommons.org/licenses/by-sa/4.0/\" style=\"color: yellow;\">"
                          " CC BY-SA 4.0</a></p>");
    imageCredits->setWordWrap(true);
    imageCredits->setTextFormat(Qt::RichText);
    imageCredits->setTextInteractionFlags(Qt::TextBrowserInteraction);
    imageCredits->setOpenExternalLinks(true);

    return imageCredits;
}

QLabel* CreditsWidget::createMusicCredits() {
    QLabel* musicCredits = new QLabel(this);
    musicCredits->setText("<h2>Songs</h2>"
                          "<p>\"<a href=\"https://freemusicarchive.org/music/David_Szesztay/20170730112627440/Sweet_Water\" style=\"color: yellow;\">"
                          "Sweet Water</a>\" by David Szesztay</p>"
                          "<p>\"<a href=\"https://freemusicarchive.org/music/Podington_Bear/Curious/Frogs\" style=\"color: yellow;\">"
                          "Frogs</a>\" by Podington Bear</p>"
                          "<p>All songs used under "
                          "<a href=\"https://creativecommons.org/licenses/by-nc/3.0/\" style=\"color: yellow;\">"
                          "CC BY-NC 3.0</a></p>");
    musicCredits->setTextFormat(Qt::RichText);
    musicCredits->setTextInteractionFlags(Qt::TextBrowserInteraction);
    musicCredits->setOpenExternalLinks(true);

    return musicCredits;
}

QLabel* CreditsWidget::createSoundCredits() {
    QLabel* soundCredits = new QLabel(this);
    soundCredits->setText("<h2>Sounds</h2>"
                          "<p>\"<a href=\"https://freesound.org/people/JonnyRuss01/sounds/478197/\" style=\"color: yellow;\">"
                          "Click_1</a>\" by JonnyRuss01</p>"
                          "<p>\"<a href=\"https://freesound.org/people/Veiler/sounds/209917/\" style=\"color: yellow;\">"
                          "Gong sabi 2</a>\" by Veiler</p>"
                          "<p>\"<a href=\"https://freesound.org/people/neolein/sounds/472707/\" style=\"color: yellow;\">"
                          "guzheng solo</a>\" by neolein</p>"
                          "<p>All sounds used under "
                          "<a href=\"https://creativecommons.org/publicdomain/zero/1.0/\" style=\"color: yellow;\">"
                          "CC0 1.0</a></p>");
    soundCredits->setTextFormat(Qt::RichText);
    soundCredits->setTextInteractionFlags(Qt::TextBrowserInteraction);
    soundCredits->setOpenExternalLinks(true);

    return soundCredits;
}

QLabel* CreditsWidget::createProgrammingCredits() {
    QLabel* programming = new QLabel(this);
    programming->setText("<h2>Programming</h2>"
                         "<p>All programming done by dair68 in "
                         "<a href=\"https://www.qt.io/\" style=\"color: yellow;\">"
                         "Qt</a></p>");
    programming->setTextFormat(Qt::RichText);
    programming->setTextInteractionFlags(Qt::TextBrowserInteraction);
    programming->setOpenExternalLinks(true);

    return programming;
}

void CreditsWidget::initializeStyleSheet() {
    setStyleSheet("CreditsWidget {"
                  "border-image: url(:/images/chinese_temple.jpg) 0 0 0 0 stretch stretch;"
                  "}"
                  "QLabel {"
                  "font-size: 24px;"
                  "color: white;"
                  "}"
                  "QPushButton {"
                  "font-size: 30px;"
                  "background-color: red;"
                  "color: white;"
                  "border: 1px solid white;"
                  "}");
}

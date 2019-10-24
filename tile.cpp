#include "tile.h"
#include <random>

unsigned Tile::SPRITE_WIDTH = 64;
unsigned Tile::SPRITE_HEIGHT = 78;

Tile::Tile() {
    id = 0;
    suit = "none";
    number = 0;
    faceup = true;
}

Tile::Tile(const unsigned id) {
    this->id = id;
    int suitNum = id / 3;

    //determing suit from the suit number
    switch (suitNum) {
    case 0:
        suit = "dot";
        break;
    case 1:
        suit = "bamboo";
        break;
    case 2:
        suit = "character";
        break;
    default:
        suit = "none";
        break;
    }

    number = (suit == "none") ? 0 : (id - suitNum * 9);
    faceup = true;
}

Tile::Tile(const std::string& suit, const unsigned number) {
    this->suit = suit;
    this->number = number;

    //finding id from suit and number
    if(suit == "dot") {
        id = number;
    }
    else if(suit == "bamboo") {
        id = 9 + number;
    }
    else if(suit == "character") {
        id = 18 + number;
    }
    //invalid suit. creating blank tile
    else {
        id=0;
        this->suit = "none";
        this->number = 0;
    }

    faceup = true;
}

unsigned Tile::getId() const {
    return id;
}

std::string Tile::getSuit() const {
    return suit;
}

unsigned Tile::getNumber() const {
    return number;
}

bool Tile::isFaceup() const {
    return faceup;
}

unsigned Tile::spriteWidth() {
    return SPRITE_WIDTH;
}

unsigned Tile::spriteHeight() {
    return SPRITE_HEIGHT;
}

bool Tile::operator==(const Tile& right) const {
    return id == right.id;
}

void Tile::render(QPainter& painter, const QRectF& target) const {
    //finding sprite based on id
    const unsigned SPRITE_SHEET_COLS = 7;

    unsigned spriteSheetX = (id % SPRITE_SHEET_COLS) * SPRITE_WIDTH;
    unsigned spriteSheetY = id / SPRITE_SHEET_COLS * SPRITE_HEIGHT;
    QRectF source = QRectF(spriteSheetX, spriteSheetY, SPRITE_WIDTH, SPRITE_HEIGHT);

    QString imagePath = ":/images/mahjong_tiles.png";
    QImage spriteSheet = QImage(imagePath);

    painter.drawImage(target, spriteSheet, source);
}

Tile randomTile() {
    int randomId = rand() % 42;
    return Tile(randomId);
}



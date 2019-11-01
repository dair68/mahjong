#include "tile.h"
#include <cassert>
#include <random>

unsigned Tile::SPRITE_WIDTH = 64;
unsigned Tile::SPRITE_HEIGHT = 78;

Tile::Tile() {
    id = 0;
    suit = "none";
    number = 0;
    faceup = true;
    selected = false;
    hovered = false;
}

Tile::Tile(const unsigned id) {
    assert(0 <= id && id < 42);

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
    selected = false;
    hovered = false;
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
    selected = false;
    hovered = false;
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

bool Tile::isSelected() const {
    return selected;
}

void Tile::toggleSelection() {
    selected = selected ? false : true;
}

void Tile::select() {
    selected = true;
}

void Tile::deselect() {
    selected = false;
}

bool Tile::isHoveredOver() const {
    return hovered;
}

void Tile::markHovered() {
    hovered = true;
}

void Tile::markNotHovered() {
    hovered = false;
}

unsigned Tile::spriteWidth() {
    return SPRITE_WIDTH;
}

unsigned Tile::spriteHeight() {
    return SPRITE_HEIGHT;
}

unsigned Tile::getX() const {
    return (id % 7) * SPRITE_WIDTH;
}

unsigned Tile::getY() const {
    return (id / 7) * SPRITE_HEIGHT;
}

bool Tile::operator==(const Tile& right) const {
    return id == right.id;
}

Tile randomTile() {
    unsigned randomId = rand() % 42;
    return Tile(randomId);
}



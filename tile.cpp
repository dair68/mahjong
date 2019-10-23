#include "tile.h"

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

bool Tile::operator==(const Tile& right) {
    return id == right.id;
}

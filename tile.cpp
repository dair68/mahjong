#include "tile.h"
#include <cassert>
#include <random>

unsigned Tile::SPRITE_WIDTH = 64;
unsigned Tile::SPRITE_HEIGHT = 78;

Tile::Tile() : id(0), suit("dragon"), number(1), faceup(true), selected(false),
    hovered(false), highlighted(false) {

}

Tile::Tile(const unsigned id) : faceup(true), selected(false), hovered(false),
highlighted(false) {
    assert(0 <= id && id < 42);
    this->id = id;

    //determing suit and number from id
    if(0 <= id && id <= 2) {
        suit = "dragon";
        number = id + 1;
    }
    else if(3 <= id && id <= 6) {
        suit = "wind";
        number = id - 2;
    }
    else if(7 <= id && id <= 10) {
        suit = "season";
        number = id - 6;
    }
    else if(11 <= id && id <= 14) {
        suit = "flower";
        number = id - 10;
    }
    else if(15 <= id && id <= 23) {
        suit = "bamboo";
        number = id - 14;
    }
    else if(24 <= id && id <= 32) {
        suit = "dot";
        number = id - 23;
    }
    else if(33 <= id && id <= 41) {
        suit = "character";
        number = id - 32;
    }
    else {
        suit = "none";
        number = 0;
    }
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

bool Tile::isHighlighted() const {
    return highlighted;
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

void Tile::setHighlight(const bool highlightStatus) {
    highlighted = highlightStatus;
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

bool Tile::operator!=(const Tile& right) const {
    return id != right.id;
}

Tile randomTile() {
    unsigned randomId = rand() % 42;
    return Tile(randomId);
}

std::list<Tile> createMahjongSet() {
    const int NUM_TILES = 162;
    std::list<Tile> tiles;

    //inserting all 162 tiles
    for(int n=0; n<42; n++) {
        //flower tiles
        if(7 <= n && n <= 14) {
            Tile tile = Tile(n);
            tiles.insert(tiles.begin(), tile);
        }
        //non flower tiles
        else {
            //adding four copies
            for(int k=1; k<=4; k++) {
                Tile tile = Tile(n);
                tiles.insert(tiles.begin(), tile);
            }
        }
    }

    return tiles;
}



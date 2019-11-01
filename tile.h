#ifndef TILE_H
#define TILE_H

#include <string>
#include <QPainter>

class Tile {
public:  
    //creates blank tile
    Tile();

    //creates a tile with a certain id
    //@param id - id number of tile. number from 0-42. tiles numbered left to right, top to bottom on tilesheet
    Tile(const unsigned id);

    //creates a tile of a certain suit and number
    //@param suit - string of the form "dot", "bamboo", or "character"
    //@param number - integer from 1 to 9
    Tile(const std::string& suit, const unsigned number);

    //gets tile id
    unsigned getId() const;

    //gets tile suit
    std::string getSuit() const;

    //gets tile number
    unsigned getNumber() const;

    //checks whether tile is face up
    bool isFaceup() const;

    //checks if tile has been selected
    bool isSelected() const;

    //selects tile if not already selected, deselects otherwise
    void toggleSelection();

    //selects tile
    void select();

    //deselects tile
    void deselect();

    //checks if tile is hovered over
    bool isHoveredOver() const;

    //marks tile as hovered
    void markHovered();

    //marks tile as not hovered
    void markNotHovered();

    //obtains width of tile on spritesheet
    static unsigned spriteWidth();

    //obtains height of tile on spritesheet
    static unsigned spriteHeight();

    //returns x coordinate of tile's topleft corner on spritesheet
    unsigned getX() const;

    //returns y coordinate of tile's topleft corner on spritesheet
    unsigned getY() const;

    //checks if two tiles have the same id i.e. the same suit and number
    //@param right - tile on right side of operator
    bool operator==(const Tile& right) const;

private:
    static unsigned SPRITE_WIDTH;
    static unsigned SPRITE_HEIGHT;
    unsigned id;
    std::string suit;
    unsigned number;
    bool faceup;
    bool selected;
    bool hovered;
};

//generates a random tile, each tile having equal probability
Tile randomTile();

#endif // TILE_H
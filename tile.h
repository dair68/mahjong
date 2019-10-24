#ifndef TILE_H
#define TILE_H

#include <string>
#include <QPainter>

class Tile {
public:  
    //creates blank tile
    Tile();

    //creates a tile with a certain id
    //@param id - id number of tile. number from 0-41. tiles numbered left to right, top to bottom on tilesheet
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

    //obtains width of tile on spritesheet
    static unsigned spriteWidth();

    //obtains height of tile on spritesheet
    static unsigned spriteHeight();

    //checks if two tiles have the same id i.e. the same suit and number
    //@param right - tile on right side of operator
    bool operator==(const Tile& right) const;

    //draws a tile on a widget
    //@param painter - QPainter object point to widget where tile will be drawn
    //@param target - rectangle where tile will be drawn
    void render(QPainter& painter, const QRectF& target) const;
private:
    static unsigned SPRITE_WIDTH;
    static unsigned SPRITE_HEIGHT;
    unsigned id;
    std::string suit;
    unsigned number;
    bool faceup;
};

//generates a random tile, each tile having equal probability
Tile randomTile();

#endif // TILE_H

#ifndef TILE_H
#define TILE_H

#include <string>

class Tile {
public:
    //creates blank tile
    Tile();

    //creates a tile with a certain id
    //@param id - id number of tile. determined by placement on tile sheet.
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

    //checks if two tiles have the same id i.e. the same suit and number
    //@param right - tile on right side of operator
    bool operator==(const Tile& right);
private:
    unsigned id;
    std::string suit;
    unsigned number;
    bool faceup;
};

#endif // TILE_H

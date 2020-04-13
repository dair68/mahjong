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

    //checks if tile highlighted
    bool isHighlighted() const;

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

    //marks tile as highlighted or not highlighted
    //@param hightlightStatus - a boolean indicating whether tile currently removable
    void setHighlight(const bool highlightStatus);

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

    //checks if two tiles have different id i.e. the different suit and/or number
    //@param right - tile on right side of operator
    bool operator!=(const Tile& right) const;

private:
    static unsigned SPRITE_WIDTH;
    static unsigned SPRITE_HEIGHT;
    unsigned id;
    std::string suit;
    unsigned number;
    bool faceup;
    bool selected;
    bool hovered;
    bool highlighted;
};

//obtains a random element from a vector
//@param vect - a nonempty vector
//returns a random element, each with equal probability
template<typename T>
T randomElement(const std::vector<T>& vect) {
    assert(vect.size() > 0);

    unsigned randIndex = rand() % vect.size();
    return vect[randIndex];
 }

//obtains a random element from a list
//@param list - a nonempty list
//returns an iterator pointing to a random element, each with equal probability
template <typename T>
typename std::list<T>::iterator randomElement(std::list<T>& list) {
    assert(list.size() > 0);

    unsigned randShift = rand() % list.size();
    typename std::list<T>::iterator iter = list.begin();

    //shifting iterator to random element
    for(int i=0; i<randShift; i++) {
        iter++;
    }

    return iter;
}

//generates a random tile, each tile having equal probability
Tile randomTile();

//creates a list of all 144 tiles within a full mahjong set
std::list<Tile> createMahjongSet();

#endif // TILE_H

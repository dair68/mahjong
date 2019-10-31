#ifndef SHISENSHO_H
#define SHISENSHO_H

#include <QWidget>
#include "mainwindow.h"
#include "tile.h"

struct Space {
    int col;
    int row;
};

//checks if two spaces have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator==(const struct Space& spaceLeft, const struct Space& spaceRight);

//checks if two spaces do not have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator!=(const struct Space& spaceLeft, const struct Space& spaceRight);

class Shisensho {
public:
    //creates 12 x 5 shisensho game
    Shisensho();

    //returns the array of tiles
    std::vector<std::vector<Tile*>> getTiles() const;

    //returns number of columns
    unsigned getCols() const;

    //returns number of rows
    unsigned getRows() const;

    //returns selected tiles
    std::vector<Tile> getSelectedTiles() const;

    //checks if a space in the grid does NOT contain a tile
    //@param space - space in the grid
    //returns true if space not in grid or does not contain tile. false otherwise
    bool spaceEmpty(const struct Space& space) const;

    //removes tile from grid
    //@param space - space containing tile to delete
    void removeTile(const struct Space& space);

    //checks if a given space exists in tile grid numbered from 0
    //@param space - some space with a column and row
    bool gridContainsSpace(const struct Space& space) const;

private:
    unsigned cols;
    unsigned rows;
    std::vector<std::vector<Tile*>> tiles;
    std::vector<Tile> selectedTiles;
};

#endif // SHISENSHO_H

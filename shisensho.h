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

    //creates game of custom size. must contain even number of tiles
    //@param cols - number of cols
    //@param rows - number of rows
    Shisensho(const unsigned cols, const unsigned rows);

    //returns the array of tiles
    std::vector<std::vector<const Tile*>> getTiles() const;

    //returns number of columns
    unsigned getCols() const;

    //returns number of rows
    unsigned getRows() const;

    //returns selected tiles
    std::vector<struct Space> getSelectedTiles() const;

    //marks a tile as hovered
    //@param space - space containing tile
    void markHovered(const struct Space& space);

    //marks a tile as not hovered
    //@param space - space containing tile
    void markNotHovered(const struct Space& space);

    //selects tile and pushes it to selectedTiles no more than 2 tiles selected total
    //@param space - space containing tile
    void selectTile(const struct Space& space);

    //deselects tile and removes it from selectedTiles
    //@param tile - pointer to a tile in grid
    void deselectTile(const struct Space& space);

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

    //finds a path between spaces with no more than two bends, if it exists.
    //@param space1 - first space. does not need to be in grid or contain tile.
    //@param space2 - second space. does not need to be in grid or contain tile.
    //path segments are vertical or horizontal lines that do not cross other tiles. empty vector if no path found.
    std::vector<struct Space> findPath(const struct Space& space1, const struct Space& space2) const;

    //deselects all tiles
    void deselectTiles();

    //removes all selected tiles from grid
    void removeSelectedTiles();

    //sees if the 2 selected tiles are matching and connected, removes them from the game if so
    //@param space1 - first space containing a tile
    //@param space2 - second space containing a tile
    void matchTiles();
private:
    unsigned cols;
    unsigned rows;
    std::vector<std::vector<Tile*>> tiles;
    std::vector<struct Space> selectedTiles;
};

#endif // SHISENSHO_H

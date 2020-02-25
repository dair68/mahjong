#ifndef SHISENSHO_H
#define SHISENSHO_H

#include <QWidget>
#include <QObject>
#include "mainwindow.h"
#include "tile.h"
#include "space.h"
#include <iostream>
#include <utility>
#include <algorithm>

class Shisensho : public QObject {
    Q_OBJECT
public:
    //creates shisensho game widget
    Shisensho();

    //creates game of custom size. must contain even number of tiles
    //@param numCols - number of cols
    //@param numRows - number of rows
    Shisensho(const unsigned numCols, const unsigned numRows);

    //copy constructor. creates a deep copy
    Shisensho(const Shisensho& game);

    //assignment operator. creates deep copy
    //@param game - Shisensho game to copy
    Shisensho& operator=(const Shisensho& game);

    //destructor
    ~Shisensho();

    //returns number of tiles currently on the field
    unsigned numTiles() const;

    //removes all the tiles in the grid
    void clearTiles();

    //creates random grid of tiles. game might not be winnable
    void createTiles();

    //resets all tiles to how they were at the beginning of game. if no games played yet, creates empty tiles
    void resetTiles();

    //creates map that maps tile id's to a vector of spaces where those tiles lie
    std::map<unsigned, std::vector<struct Space>> getTileMap() const;

    //checks if game is winnable
    bool isWinnable() const;

    //stores the current tile locations inside a .txt file.
    //file will have string appended to it. string represents array of arrays with outer array being cols, inner array being
    //row values with each value being an integer representing tile id. arrays denoted by [ ,...]. empty tiles have id 42.
    //@param filename - name of txt file to store game
    void writeToFile(const QString& filename) const;

    //creates a grid of tiles such that the game is winnable
    void createWinnableTiles();

    //returns array of all the empty spaces on grid
    std::list<struct Space> getEmptySpaces() const;

    //returns array of all spaces containing tiles
    std::vector<struct Space> getTileSpaces() const;

    //returns the array of tiles
    std::vector<std::vector<const Tile*>> getTiles() const;

    //returns array of tile ids at start of the game
    std::vector<std::vector<unsigned>> getTileIds() const;

    //returns number of columns
    unsigned getCols() const;

    //returns number of rows
    unsigned getRows() const;

    //returns selected tiles
    std::list<struct Space> getSelectedTiles() const;

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

    //checks if two spaces can be connect by uninterrupted horizontal or vertical line
    //@param space1 - first space. does not need to be in grid or contain tile
    //@param space2 - second space. does not need to be in grid or contain tile.
    //returns true if connectable by horizontal or vertical line that does not cross other tiles
    bool simplePath(const struct Space& space1, const struct Space& space2) const;

    //checks if a path does not contain any tiles other than at the endpoints
    //@param path - a sequence of at least 2 spaces
    //returns true in no tiles anywhere on path except at endpoints, false otherwise
    bool connected(const std::vector<struct Space>& path) const;

    //finds length of shortest path needed to connect a sequence of spaces
    //@param path - a sequence of at least 2 spaces
    //returns shortest length needed to connect spaces
    unsigned pathLength(const std::vector<struct Space>& path) const;

    //finds a path between spaces with no more than two bends, if it exists.
    //@param space1 - first space. does not need to be in grid or contain tile.
    //@param space2 - second space. does not need to be in grid or contain tile.
    //path segments are vertical or horizontal lines that do not cross other tiles. empty vector if no path found.
    std::vector<struct Space> findPath(const struct Space& space1, const struct Space& space2) const;

    //deselects all tiles
    void deselectTiles();

    //removes all selected tiles from grid
    void removeSelectedTiles();

    //sees if the 2 selected tiles have the correct symbols to be removable
    //@param tile1 - first tile
    //@param tile2 - second tile
    //returns true if tiles have identical ids or are both flowers/seasons, false otherwise
    bool matchingTiles(const Tile& tile1, const Tile& tile2) const;

    //checks if 2 tiles can be removed from the grid
    //@param space1 - first space. must contain a tile
    //@param space2 - second space. must contain a tile
    //returns true if tiles having matching symbols and an appropriate path exists
    bool removableTiles(const struct Space& space1, const struct Space& space2) const;

    //checks if there are any removable pairs on the grid
    bool hasRemovableTiles() const;

    //checks if game over due to all tiles removed or no more moves available
    bool isOver() const;

    //checks if there are any tiles left in grid
    bool tilesLeft() const;

signals:
    //emit when the object has initialized all the tiles
    void gameInitialized();
private:
    //turns this object into a deep copy
    //@param game - shisensho game to copy
    void copy(const Shisensho& game);

    unsigned cols;
    unsigned rows;
    std::vector<std::vector<Tile*>> tiles;
    std::vector<std::vector<unsigned>> tileIds;
    std::list<struct Space> selectedTiles;
};

//partitions vector into 2 based on a set of indexes
//@param vect - vector to partition
//@param indexes - a vector containing the indexes of all elements to add to vector1
//returns a pair of vectors, with 1st vector containing all elements indicated by indexes
template <typename T>
struct std::pair<std::vector<T>, std::vector<T>> partition(
        const std::vector<T>& vect,
        const std::vector<unsigned>& indexes
    ) {
    std::vector<T> v1;
    std::vector<T> v2;

    //partitioning elements
    for(int i=0; i<vect.size(); i++) {
        T element = vect[i];
        auto iter = find(indexes.begin(), indexes.end(), i);

        //i contained in indexes; adding to v1
        if(iter != indexes.end()) {
            v1.push_back(element);
        }
        //i not contained in indexes; adding to v2
        else {
            v2.push_back(element);
        }
    }

    return std::make_pair(v1, v2);
}

#endif // SHISENSHO_H

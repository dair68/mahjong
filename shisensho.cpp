#include "shisensho.h"
#include <vector>
#include <QPainter>
#include <time.h>
#include <cassert>

bool operator==(const struct Space& spaceLeft, const struct Space& spaceRight) {
    return spaceLeft.col == spaceRight.col && spaceLeft.row == spaceRight.row;
}

bool operator!=(const struct Space& spaceLeft, const struct Space& spaceRight) {
    return spaceLeft.col != spaceRight.col || spaceLeft.row != spaceRight.row;
}

Shisensho::Shisensho() {
    cols = 12;
    rows = 5;
    tiles = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    //setting seed for random tiles
    srand(time(NULL));

    //generating random tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = new Tile(randomTile());
        }
    }

    selectedTiles = std::vector<struct Space>();
}

Shisensho::Shisensho(const unsigned cols, const unsigned rows) {
    assert((cols * rows) % 2 == 0);

    this->cols = cols;
    this->rows = rows;
    tiles = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = new Tile(4);
        }
    }

    selectedTiles = std::vector<struct Space>();
}

std::vector<std::vector<const Tile*>> Shisensho::getTiles() const {
    auto tilesCopy = std::vector<std::vector<const Tile*>>(cols, std::vector<const Tile*>(rows));

    //creating defensive copy
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            //space contains a tile
            if(tiles[i][j] != nullptr) {
                tilesCopy[i][j] = tiles[i][j];
            }
            //space does not contain tile
            else {
                tilesCopy[i][j] = nullptr;
            }
        }
    }

   return tilesCopy;
}

unsigned Shisensho::getCols() const {
    return cols;
}

unsigned Shisensho::getRows() const {
    return rows;
}

std::vector<struct Space> Shisensho::getSelectedTiles() const {
    auto selected = std::vector<struct Space>();

    //copy pointers as const pointers
    for(int i=0; i<selectedTiles.size(); i++) {
        const int col = selectedTiles[i].col;
        const int row = selectedTiles[i].row;
        const struct Space space = {col, row};
        selected.push_back(space);
    }

    return selected;
}

void Shisensho::markHovered(const struct Space& space) {
    assert(gridContainsSpace(space));
    assert(tiles[space.col][space.row] != nullptr);

    Tile& tile = *tiles[space.col][space.row];
    tile.markHovered();
}

void Shisensho::markNotHovered(const struct Space& space) {
    assert(gridContainsSpace(space));
    assert(tiles[space.col][space.row] != nullptr);

    Tile& tile = *tiles[space.col][space.row];
    tile.markNotHovered();
}

void Shisensho::selectTile(const struct Space& space) {
    assert(gridContainsSpace(space));
    assert(tiles[space.col][space.row] != nullptr);

    //checking if less than 2 tiles selected
    if(selectedTiles.size() < 2) {
        Tile& tile = *tiles[space.col][space.row];
        tile.select();
        selectedTiles.push_back(space);
    }
}

void Shisensho::deselectTile(const struct Space& space) {
    assert(gridContainsSpace(space));
    assert(tiles[space.col][space.row] != nullptr);

    Tile& tile = *tiles[space.col][space.row];
    tile.deselect();

    //tile is first selected tile
    if(selectedTiles[0] == space) {
        selectedTiles[0] = selectedTiles[1];
        selectedTiles.pop_back();
    }
    //tile is second selected tile
    else {
        selectedTiles.pop_back();
    }
}

bool Shisensho::spaceEmpty(const struct Space& space) const {
    //space not in grid
    if(!gridContainsSpace(space)) {
        return true;
    }

    return tiles[space.col][space.row] == nullptr;
}


void Shisensho::removeTile(const struct Space& space) {
    assert(gridContainsSpace(space));
    assert(tiles[space.col][space.row] != nullptr);

    //checking for valid space
    if(!spaceEmpty(space)) {
        delete tiles[space.col][space.row];
        tiles[space.col][space.row] = nullptr;
    }
}

bool Shisensho::gridContainsSpace(const struct Space& space) const {
    int row = space.row;
    int col = space.col;
    return  0 <= row && row < rows && 0 <= col && col < cols;
}

std::vector<struct Space> Shisensho::findPath(const struct Space& space1, const struct Space& space2) const {
    //ensuring first argument is higher space
    if(space1.row > space2.row) {
        return findPath(space2, space1);
    }
    //ensuring first argument is leftmost space if sharing same row
    else if(space1.row == space2.row && space1.col > space2.col) {
        return findPath(space2, space1);
    }

    std::vector<struct Space> path;

    //spaces share same column
    if(space1.col == space2.col) {
        //checking if every space between spaces are vacant
        for(int j=space1.row + 1; j<space2.row; j++) {
            //found occupied space. spaces not connected
            if(tiles[space1.col][j] != nullptr) {
                return path;
            }
        }
        path = {space1, space2};
        return path;
    }
}

void Shisensho::deselectTiles() {
    //deselecting each selected tile
    for(int i=0; i<selectedTiles.size(); i++) {
        struct Space selected = selectedTiles[i];
        Tile& tile = *tiles[selected.col][selected.row];
        tile.deselect();
    }

    selectedTiles = std::vector<struct Space>();
}

void Shisensho::removeSelectedTiles() {
    //removing each selected tile
    for(int i=0; i<selectedTiles.size(); i++) {
        struct Space selected = selectedTiles[i];
        removeTile(selected);
    }

    selectedTiles = std::vector<struct Space>();
}

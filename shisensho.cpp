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

    selectedTiles = std::vector<Tile*>();
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

std::vector<const Tile*> Shisensho::getSelectedTiles() const {
    auto selected = std::vector<const Tile*>(selectedTiles.size());

    //copy pointers as const pointers
    for(int i=0; i<selectedTiles.size(); i++) {
        selected[i] = selectedTiles[i];
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
        selectedTiles.push_back(&tile);
    }
}

void Shisensho::deselectTile(const struct Space& space) {
    Tile& tile = *tiles[space.col][space.row];
    tile.deselect();

    //tile is first element
    if(selectedTiles[0] == &tile) {
        selectedTiles[0] = selectedTiles[1];
        selectedTiles.pop_back();
    }
    //tile is second element
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
    //checking for valid space
    if(!spaceEmpty(space)) {
        delete tiles[space.col][space.row];
        tiles[space.col][space.row] = nullptr;
    }
}

bool Shisensho::gridContainsSpace(const struct Space &space) const {
    int row = space.row;
    int col = space.col;
    return  0 <= row && row < rows && 0 <= col && col < cols;
}



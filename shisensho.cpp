#include "shisensho.h"
#include <QPainter>
#include <time.h>
#include <math.h>
#include <QMouseEvent>

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
    for(int i=0; i<cols; i=i+2) {
        for(int j=0; j<rows; j++) {
            Tile* tile = new Tile(randomTile());
            Tile* tileCopy = new Tile(*tile);

            tiles[i][j] = tile;
            tiles[i+1][j] = tileCopy;
        }
    }

    selectedTiles = std::vector<Tile>();
}

std::vector<std::vector<Tile*>> Shisensho::getTiles() const {
    auto tilesCopy = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    //creating defensive copy
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            //tile exists at space
            if(tiles[i][j] != nullptr) {
                Tile* tile = new Tile(*tiles[i][j]);
                tilesCopy[i][j] = tile;
            }
            //no tile at space
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

std::vector<Tile> Shisensho::getSelectedTiles() const {
    //creating defensive copy
    std::vector<Tile> copy = std::vector<Tile>(selectedTiles.size());

    return selectedTiles;
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



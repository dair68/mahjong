#include "shisensho.h"
#include <vector>
#include <QPainter>
#include <ctime>
#include <cassert>
#include <QDebug>

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
    auto selected = std::vector<struct Space>(selectedTiles.size());

    //copy pointers as const pointers
    for(int i=0; i<selectedTiles.size(); i++) {
        const int col = selectedTiles[i].col;
        const int row = selectedTiles[i].row;
        const struct Space space = {col, row};
        selected[i] = space;
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

bool Shisensho::simplePath(const struct Space &space1, const struct Space &space2) const {
    int col1 = space1.col;
    int col2 = space2.col;
    int row1 = space1.row;
    int row2 = space2.row;

    //ensuring that space1 if topmost space
    if(row1 > row2) {
        return simplePath(space2, space1);
    }

    //spaces share column
    if(col1 == col2) {
        //checking if there's no tiles between spaces
        for(int j=row1 + 1; j<row2; j++) {
            //found tile. spaces not simply connected
            if(!spaceEmpty({col1,j})) {
                return false;
            }
        }
        return true;
    }

    //ensuring that space1 is leftmost space
    if(col1 > col2) {
        return simplePath(space2, space1);
    }

    //spaces on same row
    if(row1 == row2) {
        //checking if there's no tiles between spaces
        for(int i=col1 + 1; i<col2; i++) {
            //found tile. spaces not simply connected
            if(!spaceEmpty({i,row2})) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Shisensho::connected(const std::vector<struct Space>& path) const {
    assert(path.size() >= 2);

    //checking if spaces between endpoints are vacant
    for(int i=1; i<path.size()-1; i++) {
        struct Space space = path[i];
        //space contains tile, path not connected
        if(!spaceEmpty(space)) {
            return false;
        }
    }

    //checking if path segments contain no tiles
    for(int i=0; i<path.size()-1; i++) {
        struct Space space1 = path[i];
        struct Space space2 = path[i + 1];

        //not a simple path, path not connected
        if(!simplePath(space1, space2)) {
            return false;
        }
    }

    return true;
}

unsigned Shisensho::pathLength(const std::vector<struct Space>& path) const {
    assert(path.size() >= 2);

    unsigned pathLength = 0;

    //calculating min path length
    for(int i=0; i<path.size() - 1; i++) {
        struct Space space1 = path[i];
        struct Space space2 = path[i + 1];

        int length = abs(space1.row - space2.row) + abs(space1.col - space2.col);
        pathLength += length;
    }

    return pathLength;
}

std::vector<struct Space> Shisensho::findPath(const struct Space& space1, const struct Space& space2) const {
    int col1 = space1.col;
    int col2 = space2.col;
    int row1 = space1.row;
    int row2 = space2.row;

    //ensuring first argument is higher space
    if(row1 > row2) {
        return findPath(space2, space1);
    }
    //ensuring first argument is leftmost space if sharing same row
    else if(row1 == row2 && col1 > col2) {
        return findPath(space2, space1);
    }

    std::vector<struct Space> path;

    //spaces share same column
    if(col1 == col2) {
        //spaces are simply connected, path found
        if(simplePath(space1, space2)) {
            qDebug() << "simple path";
            path = {space1, space2};
            return path;
        }

        //searching for path of form '[' or ']'
        for(int i=-1; i<=(int)cols; i++) {
            struct Space topCorner = {i, row1};
            struct Space bottomCorner = {i, row2};

            std::vector<struct Space> path2 = {space1, topCorner, bottomCorner, space2};

            //found a path!
            if(connected(path2) && (path.size() == 0 || pathLength(path2) < pathLength(path))) {
                path = path2;
            }
        }
        return path;
    }

    //spaces share same row
    if(row1 == row2) {
        //checking if direct path between spaces
        if(simplePath(space1, space2)) {
            path = {space1, space2};
            return path;
        }

        //looking for paths type |-| or |_|
        for(int j=-1; j<=(int)rows; j++) {
            struct Space leftCorner = {col1, j};
            struct Space rightCorner = {col2, j};
            std::vector<struct Space> path2 = {space1, leftCorner, rightCorner, space2};

            //found path
            if(connected(path2) && (path.size() == 0 || pathLength(path2) < pathLength(path))) {
                path = path2;
            }
        }
        return path;
    }

    return path;
}

void Shisensho::deselectTiles() {
    //deselecting each selected tile
    for(int i=0; i<selectedTiles.size(); i++) {
        struct Space selected = selectedTiles[i];
        Tile& tile = *tiles[selected.col][selected.row];
        tile.deselect();
    }

    selectedTiles.clear();
}

void Shisensho::removeSelectedTiles() {
    //removing each selected tile
    for(int i=0; i<selectedTiles.size(); i++) {
        struct Space selected = selectedTiles[i];
        removeTile(selected);
    }

    selectedTiles = std::vector<struct Space>();
}

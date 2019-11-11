#include "tileset.h"

TileSet::TileSet() {
    tiles = std::vector<Tile>(144);
    int i = 0;

    //inserting all 144 tiles
    for(int n=0; n<42; n++) {
        //flower/season tiles
        if(7 <= n && n <= 14) {
            Tile tile = Tile(n);
            tiles[i] = tile;
            i++;
        }
        //non flower tiles
        else {
            //adding four copies
            for(int k=1; k<=4; k++) {
                Tile tile = Tile(n);
                tiles[i] = tile;
                i++;
            }
        }
    }
}

Tile& TileSet::randomTile() {
    unsigned randIndex = rand() % tiles.size();
    return tiles[randIndex];
}

Tile* TileSet::findTile(const unsigned id) {
    assert(0 <= id && id < 42);

    unsigned currentId;
    unsigned lowerBound = 0;
    unsigned upperBound = tiles.size() - 1;

    //performing binary search
    while(currentId != id) {
        unsigned mid = (lowerBound + upperBound) / 2;
        Tile& tile = tiles[mid];
        currentId = tile.getId();

        //found tile!
        if(id == currentId) {
            return &tile;
        }
        //bounds have converged with still no tile; tile DNE
        else if(lowerBound == upperBound) {
            return nullptr;
        }
        //tile in left half
        else if(id < currentId) {
            upperBound = mid;
        }
        //tile in right half
        else {
            lowerBound = mid + 1;
        }
    }
}

std::vector<Tile&> TileSet::findSuit(const std::string& suit) {
    std::vector<Tile&> suitTiles;

    //searching for tiles that satisfy suit
    for(int i=0; i<tiles.size(); i++) {
        Tile& tile = tiles[i];

        //tile satisfies suit
        if(tile.getSuit() == suit) {
            suitTiles.push_back(tile);
        }
    }

    return suitTiles;
}

void TileSet::removeTile(const unsigned id) {
    std::vector<Tile>::iterator iter = tiles.begin();

    //searching for tile
    while(iter->getId() != id) {
       iter++;
    }

    //tile found
    if(iter != tiles.end()) {
        tiles.erase(iter);
    }
}

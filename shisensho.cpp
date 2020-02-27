#include "shisensho.h"
#include "space.h"
#include <fstream>
#include <QFile>
#include <QDir>
#include <vector>
#include <QPainter>
#include <ctime>
#include <cassert>
#include <QDebug>

Shisensho::Shisensho() : cols(12), rows(5), selectedTiles() {
    tiles = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    //creating empty grid
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = nullptr;
        }
    }

    tileIds = std::vector<std::vector<unsigned>>(cols, std::vector<unsigned>(rows, 42));
}

Shisensho::Shisensho(const unsigned numCols, const unsigned numRows)
    : cols(numCols), rows(numRows), selectedTiles() {
    assert((cols * rows) % 2 == 0);

    tiles = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    //creating empty grid
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = nullptr;
        }
    }

    tileIds = std::vector<std::vector<unsigned>>(cols, std::vector<unsigned>(rows, 42));
}

Shisensho::Shisensho(const Shisensho& game) {
   copy(game);
}

Shisensho& Shisensho::operator=(const Shisensho& game) {
    //not self assignment
    if(this != &game) {
        clearTiles();
        copy(game);
    }

    return *this;
}

Shisensho::~Shisensho() {
    clearTiles();
}

unsigned Shisensho::numTiles() const {
    unsigned numTiles = 0;

    //counting tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            //tile exists
            if(tiles[i][j] != nullptr) {
                numTiles++;
            }
        }
    }

    return numTiles;
}

void Shisensho::clearTiles() {
    //removing every tile in grid
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
          //tile to delete
            if(tiles[i][j] != nullptr) {
                delete tiles[i][j];
                tiles[i][j] = nullptr;
            }
        }
    }
}

std::map<unsigned, std::vector<struct Space>> Shisensho::getTileMap() const {
    std::map<unsigned, std::vector<struct Space>> tileMap;

    //parsing tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            //tile exists
            if(tiles[i][j] != nullptr) {
                Tile* tile = tiles[i][j];
                unsigned id = tile->getId();
                struct Space space = {i, j};
                assert(tiles[space.col][space.row]);

                //all flower tiles assigned id of 7
                if(7 <= id && id <= 10) {
                    id = 7;
                }
                //all season tiles assigned id of 11
                else if(11 <= id && id <= 14) {
                    id = 11;
                }

                auto iter = tileMap.find(id);

                //key found
                if(iter != tileMap.end()) {
                    std::vector<struct Space>& spaces = iter->second;
                    spaces.push_back(space);
                }
                //key not found
                else {
                    std::vector<struct Space> spaces;
                    spaces.push_back(space);
                    tileMap[id] = spaces;
                }
            }
        }
    }

    return tileMap;
}


bool Shisensho::isWinnable() const {
    //qDebug() << "checking";
    Shisensho copy = *this;
    unsigned numTiles = copy.numTiles();

    //searching for safely removable tiles
    while(copy.hasRemovableTiles()) {
        //qDebug() << "pruning";
        //qDebug() << numTiles << "tiles left";
        std::map<unsigned, std::vector<struct Space>> tileMap = copy.getTileMap();

        //searching for pairs of removable tiles to do not affect game outcome
        for(auto& [id, spaces] : tileMap) {
            //qDebug() << "checking tiles " << id;

            //only 2 of a given tile
            if(spaces.size() == 2) {
                //qDebug() << spaces.size();
                struct Space space1 = spaces[0];
                struct Space space2 = spaces[1];

                //tiles removable; removing them
                if(copy.removableTiles(space1, space2)) {
                    copy.removeTile(space1);
                    copy.removeTile(space2);
                }
            }
            //4 of a given tile
            else {
                //qDebug() << spaces.size();
                std::vector<std::vector<unsigned>> partitionIndexes = {{0,1}, {0,2}, {0,3}};

                //examining different pairs of spaces
                for(std::vector<unsigned> indexes : partitionIndexes) {
                   struct std::pair<std::vector<struct Space>, std::vector<struct Space>> spaceVects;
                   spaceVects = partition(spaces, indexes);

                   std::vector<struct Space> spacePair1 = spaceVects.first;
                   struct Space spaceA = spacePair1[0];
                   //qDebug() << tiles[spaceA.col][spaceA.row]->getId();
                   struct Space spaceB = spacePair1[1];

                   std::vector<struct Space> spacePair2 = spaceVects.second;
                   struct Space spaceC = spacePair2[0];
                   struct Space spaceD = spacePair2[1];

                   //possible to remove all 4 tiles
                   if(copy.removableTiles(spaceA, spaceB) && copy.removableTiles(spaceC, spaceD)) {
                      //removing all 4 tiles
                       for(struct Space space : spaces) {
                           copy.removeTile(space);
                       }
                       break;
                   }
                }
            }
        }

        //qDebug() << copy.numTiles() << numTiles;
        //no more tiles to prune
        if(copy.numTiles() == numTiles) {
            break;
        }
        //tiles pruned
        else {
            numTiles = copy.numTiles();
        }
    }

    //no tiles left, game is winnable
    if(!copy.tilesLeft()) {
        return true;
    }

    std::map<unsigned, std::vector<struct Space>> tileMap = copy.getTileMap();

    //searching for any pair of removable tiles, including those that might end game prematurely
    for(auto& [id, spaces] : tileMap) {
        //trying to remove different pairs of tiles
        for(int a=0; a<spaces.size()-1; a++) {
            for(int b=a+1; b<spaces.size(); b++) {
                struct Space space1 = spaces[a];
                struct Space space2 = spaces[b];

                //removable tiles
                if(copy.removableTiles(space1, space2)) {
                    //qDebug() << "guess";
                    Shisensho trialGame = copy;
                    trialGame.removeTile(space1);
                    trialGame.removeTile(space2);

                    //game is winnable
                    if(trialGame.isWinnable()) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Shisensho::createTiles() {
    qDebug() << "creating tiles";
    int tilePairs = (cols * rows) / 2;
    std::list<Tile> tileSet = createMahjongSet();
    int pairs = 0;

    //filling board randomly
    while(pairs < tilePairs) {
        std::list<struct Space> emptySpaces = getEmptySpaces();
        std::list<struct Space>::iterator spaceIter = randomElement(emptySpaces);
        struct Space space1 = *spaceIter;

        std::list<Tile>::iterator tileIter = randomElement(tileSet);
        Tile* tile1 = new Tile(*tileIter);
        tiles[space1.col][space1.row] = tile1;
        tileSet.erase(tileIter);

        emptySpaces.erase(spaceIter);
        spaceIter = randomElement(emptySpaces);
        struct Space space2 = *spaceIter;

        std::list<Tile> matches;

        //searching for matching tiles in tileset
        for(Tile tile : tileSet) {
            //found a match
            if(matchingTiles(tile, *tile1)) {
                matches.push_back(tile);
            }
        }

        std::list<Tile>::iterator matchIter = randomElement(matches);
        Tile* tile2 = new Tile(*matchIter);
        tiles[space2.col][space2.row] = tile2;

        tileIter = std::find(tileSet.begin(), tileSet.end(), *tile2);
        tileSet.erase(tileIter);

        pairs++;
    }
}

void Shisensho::resetTiles() {
    //resetting tiles based on tileIds recorded earlier
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            unsigned id = tileIds[i][j];

            //id is 42, creating empty tile
            if(id == 42) {
                tiles[i][j] = nullptr;
            }
            //creating tile based on id
            else {
                tiles[i][j] = new Tile(id);
            }
        }
    }
}

void Shisensho::createWinnableTiles() {
   createTiles();

   //randomly generating grids until winnable grid found
   while(!isWinnable()) {
       clearTiles();
       createTiles();
   }

   //updating tile ids
   for(int i=0; i<cols; i++) {
       for(int j=0; j<rows; j++) {
           Tile tile = * tiles[i][j];
           unsigned id = tile.getId();
           tileIds[i][j] = id;
       }
   }

   //qDebug() << "done";
   emit gameInitialized();
}

QString Shisensho::gridStatusString() const {
    QStringList columnData;

    //adding game tiles to string
    for(int i=0; i<cols; i++) {
        QString colString = "[";
        for(int j=0; j<rows; j++) {
            Tile* tile = tiles[i][j];
            unsigned id = (tile == nullptr) ? 42 : tile->getId();
            colString += QString::number(id);
            //adding comma if needed
            if(j+1 != rows) {
                colString += ",";
            }
        }
        colString += "]";
        columnData.append(colString);
    }

    return "[" + columnData.join(",") + "]";
}

std::list<struct Space> Shisensho::getEmptySpaces() const {
    std::list<struct Space> emptySpaces;

    //finding empty spaces
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            struct Space space = {i,j};
            //found empty space
            if(tiles[i][j] == nullptr) {
                emptySpaces.push_back(space);
            }
        }
    }

    return emptySpaces;
}

std::vector<struct Space> Shisensho::getTileSpaces() const {
    std::vector<struct Space> tileSpaces;

    //finding tile spaces
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            struct Space space = {i, j};
            //found space containing tile
            if(tiles[i][j] != nullptr) {
                tileSpaces.push_back(space);
            }
        }
    }

    return tileSpaces;
}

std::vector<std::vector<const Tile*>> Shisensho::getTiles() const {
    auto tilesCopy = std::vector<std::vector<const Tile*>>(cols, std::vector<const Tile*>(rows));

    //creating pointers to tiles
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

std::vector<std::vector<unsigned>> Shisensho::getTileIds() const {
    return tileIds;
}

unsigned Shisensho::getCols() const {
    return cols;
}

unsigned Shisensho::getRows() const {
    return rows;
}

std::list<struct Space> Shisensho::getSelectedTiles() const {
    return selectedTiles;
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

    auto iter = find(selectedTiles.begin(), selectedTiles.end(), space);

    //found selected space and erasing
    if(iter != selectedTiles.end()) {
        selectedTiles.erase(iter);
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
    assert(tiles[space.col][space.row] != nullptr);

    delete tiles[space.col][space.row];
    tiles[space.col][space.row] = nullptr;
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
            //qDebug() << "simple path";
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

    //searching for an L-shaped path
    struct Space corner1 = {col2, row1};
    struct Space corner2 = {col1, row2};
    std::vector<struct Space> lPath1 = {space1, corner1, space2};
    std::vector<struct Space> lPath2 = {space1, corner2, space2};

    //found path
    if(connected(lPath1)) {
        return lPath1;
    }
    else if(connected(lPath2)) {
        return lPath2;
    }

    //searching for path with horizontal, vertical, then horizontal segment
    for(int i=-1; i<=(int)cols; i++) {
        struct Space topCorner = {i, row1};
        struct Space bottomCorner = {i, row2};
        std::vector<struct Space> path2 = {space1, topCorner, bottomCorner, space2};

        //found new viable path
        if(connected(path2) && (path.size() == 0 || pathLength(path2) < pathLength(path))) {
            path = path2;
        }
    }

    //searching for a path with vertical, horizontal, then vertical segment
    for(int j=-1; j<=(int)rows; j++) {
        struct Space leftCorner = {col1, j};
        struct Space rightCorner = {col2, j};
        std::vector<struct Space> path2 = {space1, leftCorner, rightCorner, space2};

        //found new viable path
        if(connected(path2) && (path.size() == 0 || pathLength(path2) < pathLength(path))) {
            path = path2;
        }
    }

    return path;
}

void Shisensho::deselectTiles() {
    //deselecting each selected tile
    for(struct Space space : selectedTiles) {
        Tile& tile = *tiles[space.col][space.row];
        tile.deselect();
    }
    selectedTiles.clear();
}

void Shisensho::removeSelectedTiles() {
    //removing each selected tile
    for(struct Space space : selectedTiles) {
        removeTile(space);
    }

    selectedTiles.clear();
}

bool Shisensho::matchingTiles(const Tile& tile1, const Tile& tile2) const {
    //tiles have same symbol
    if(tile1 == tile2) {
        return true;
    }
    //both tiles are flowers
    else if(tile1.getSuit() == "flower" && tile2.getSuit() == "flower") {
        return true;
    }
    //both tiles are seasons
    else if(tile1.getSuit() == "season" && tile2.getSuit() == "season") {
        return true;
    }

    return false;
}

bool Shisensho::removableTiles(const struct Space& space1, const struct Space& space2) const {
    assert(tiles[space1.col][space1.row] != nullptr && tiles[space2.col][space2.row] != nullptr);

    std::vector<Space> path = findPath(space1, space2);
    Tile tile1 = *tiles[space1.col][space1.row];
    Tile tile2 = *tiles[space2.col][space2.row];

    return path.size() > 0 && matchingTiles(tile1, tile2);
}

bool Shisensho::hasRemovableTiles() const {
    std::map<unsigned, std::vector<struct Space>> tileMap = getTileMap();

    //examining tiles by suit
    for(auto& [id, spaces] : tileMap) {
        //trying to remove different pairs of tiles
        for(int a=0; a<spaces.size()-1; a++) {
            for(int b=a+1; b<spaces.size(); b++) {
                struct Space space1 = spaces[a];
                struct Space space2 = spaces[b];

                //removable tiles
                if(removableTiles(space1, space2)) {
                   return true;
                }
            }
        }
    }

    return false;
}

bool Shisensho::isOver() const {
    return !hasRemovableTiles();
}

bool Shisensho::tilesLeft() const {
    //searching for leftover tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            Tile* tile = tiles[i][j];

            //found tile
            if(tile != nullptr) {
                //qDebug() << "still tiles";
                return true;
            }
        }
    }

    qDebug() << "no more tiles!";
    return false;
}

void Shisensho::copy(const Shisensho& game) {
    cols = game.cols;
    rows = game.rows;
    tiles = std::vector<std::vector<Tile*>>(cols, std::vector<Tile*>(rows));

    //copying over all tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            //tile exists
            if(game.tiles[i][j] != nullptr) {
                Tile tile = *(game.tiles[i][j]);
                tiles[i][j] = new Tile(tile);
            }
            //tile does not exist
            else {
                tiles[i][j] = nullptr;
            }
        }
    }

    selectedTiles = game.selectedTiles;
    tileIds = game.tileIds;
}

void generateShisenshoGames(const unsigned numGames, const QString& filename, const QString& gameSize = "small") {
    QSet<QString> levels;

    QString filepath = QDir::currentPath() + "/" + filename;
    QFile file (filepath);

    //checking that file opened correctly
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can't open file";
        return;
    }

    //reading levels from file
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        levels.insert(line);
    }
    file.close();

    Shisensho game;

    //small, 12x5 game
    if(gameSize == "small") {
        game = Shisensho(12, 5);
    }
    //medium, 14x6 game
    else if(gameSize == "medium") {
        game = Shisensho(14, 6);
    }
    //large, 18x8 game
    else if(gameSize == "large") {
        game = Shisensho(18, 8);
    }
    //invalid size
    else {
        qDebug() << "invalid game size";
        return;
    }

    for(int i=0; i<numGames; i++) {
        game.createWinnableTiles();
        QString levelData = game.gridStatusString();

        //refreshing game if duplicate level already stored
        while(levels.contains(levelData)) {
            game.clearTiles();
            game.createWinnableTiles();
            levelData = game.gridStatusString();
        }
        writeToFile(levelData + "\n", filename);
        game.clearTiles();
    }
}

void writeToFile(const QString& data, const QString& filename) {
    QString filePath = QDir::currentPath() + "/" + filename;
    qDebug() << filePath;
    QFile file(filePath);

    //checking that file opened correctly
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "can't open file";
        return;
    }

    QTextStream out(&file);
    out << data;
    qDebug() << "done writing";
    file.close();
}

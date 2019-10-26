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

Shisensho::Shisensho(MainWindow* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);

    cols = 12;
    rows = 5;
    tiles = std::vector<std::vector<Tile>>(cols, std::vector<Tile>(rows));
    tilesDrawn = false;
    backgroundDrawn = false;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};

    //setting seed for random tiles
    srand(time(NULL));

    //generating random tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = randomTile();
        }
    }

    setMouseTracking(true);
}

void Shisensho::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //checking if background needs to be drawn
    if(!backgroundDrawn) {
        QBrush brush = QBrush(Qt::darkGreen);
        painter.fillRect(0, 0, width(), height(), brush);
        backgroundDrawn = true;
    }

    //checking if tiles need to be drawn
    if(!tilesDrawn) {
        drawTiles(painter);
        tilesDrawn = true;
    }

    //checking if a tile needs to be repainted
    if(gridContainsSpace(updatedSpace)) {
        redrawTile(painter, updatedSpace); 
        updatedSpace = {-1, -1};
    }

    //checking if any newly hovered tiles
    if(gridContainsSpace(hoveredSpace)) {
        redrawTile(painter, hoveredSpace);
    }
}

void Shisensho::drawTiles(QPainter& painter) const {
    int tileWidth = 54;
    int tileHeight = 65;
    int topCornerX = (width() - tileWidth*cols)/2;
    int topCornerY = (height() - tileHeight*rows)/2;

    //drawing tiles
    for(int i=0; i<cols; i++) {
        for(int j=rows-1; j>=0; j--) {
            Tile tile = tiles[i][j];

            int targetX = topCornerX + i*tileWidth;
            int targetY = topCornerY + j*tileHeight;
            QRectF target(targetX, targetY, Tile::spriteWidth(), Tile::spriteHeight());

            QString tilesheetPath = ":/images/mahjong_tiles.png";
            QImage spriteSheet = QImage(tilesheetPath);
            QRectF source(tile.getX(), tile.getY(), Tile::spriteWidth(), Tile::spriteHeight());

            painter.drawImage(target, spriteSheet, source);
        }
    }
}

void Shisensho::redrawTile(QPainter& painter, const struct Space& space) {
    int tileWidth = 54;
    int tileHeight = 65;
    int verticalShift = Tile::spriteHeight() - tileHeight;
    int topCornerX = (width() - tileWidth*cols)/2;
    int topCornerY = (height() - tileHeight*rows)/2 + verticalShift;

    int targetX = topCornerX + tileWidth * space.col;
    int targetY = topCornerY + tileHeight * space.row;
    QRect target = QRect(targetX, targetY, tileWidth, tileHeight);

    Tile& tile = tiles[space.col][space.row];
    QString imagePath;

    //tile hovered over
    if(space == hoveredSpace) {
        QString hoveredTilePath = ":/images/dim_tiles.png";
        QString hoveredSelectedTilePath = ":/images/dim_selected_tiles.png";
        imagePath = tile.isSelected() ? hoveredSelectedTilePath : hoveredTilePath;
    }
    else {
        QString selectedTilePath = ":/images/selected_tiles.png";
        QString unselectedTilePath = ":/images/mahjong_tiles.png";
        imagePath = tile.isSelected() ? selectedTilePath : unselectedTilePath;
    }

    QImage spritesheet = QImage(imagePath);

    QRect source = QRect(tile.getX(), tile.getY() + verticalShift, tileWidth, tileHeight);
    painter.drawImage(target, spritesheet, source);
}

bool Shisensho::gridContainsSpace(const struct Space &space) const {
    int row = space.row;
    int col = space.col;
    return  0 <= row && row < rows && 0 <= col && col < cols;
}

struct Space Shisensho::findSpace(const unsigned x, const unsigned y) const {
    int tileWidth = 54;
    int tileHeight = 65;
    int topCornerX = (width() - tileWidth*12)/2;
    int topCornerY = (height() - tileHeight*5)/2 + Tile::spriteHeight() - tileHeight;

    int col = floor((double)(x - topCornerX) / tileWidth);
    int row = floor((double)(y - topCornerY) / tileHeight);
    struct Space space = {col, row};

    return space;
}

void Shisensho::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();

    struct Space newHoveredSpace = findSpace(x,y);

    //mousing hovering over tile
    if(gridContainsSpace(newHoveredSpace) && hoveredSpace != newHoveredSpace) {
        //mouse left previous tile
        if(gridContainsSpace(hoveredSpace)) {
            Tile& prevHoveredTile = tiles[hoveredSpace.col][hoveredSpace.row];
            prevHoveredTile.markNotHovered();
            updatedSpace = hoveredSpace;
        }

        hoveredSpace = newHoveredSpace;
        Tile& hoveredTile = tiles[hoveredSpace.col][hoveredSpace.row];
        hoveredTile.markHovered();
        repaint();
    }
    //mouse went from tile to empty space
    else if(!gridContainsSpace(newHoveredSpace) && gridContainsSpace(hoveredSpace)) {
        Tile& hoveredTile = tiles[hoveredSpace.col][hoveredSpace.row];
        hoveredTile.markNotHovered();
        updatedSpace = hoveredSpace;
        hoveredSpace = newHoveredSpace;
        repaint();
    }
}

void Shisensho::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    struct Space clickedSpace = findSpace(x, y);

    //checking if tile needs to be updated
    if(gridContainsSpace(clickedSpace)) {
        Tile& clickedTile = tiles[clickedSpace.col][clickedSpace.row];
        clickedTile.toggleSelection();

        updatedSpace = clickedSpace;
        repaint();
    }
}

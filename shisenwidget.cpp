#include "shisenwidget.h"
#include <QPainter>
#include <math.h>
#include <QMouseEvent>

ShisenWidget::ShisenWidget(MainWindow* parent) : QWidget(parent) {
    game = Shisensho();
    tilesDrawn = false;
    backgroundDrawn = false;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};

    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    this->setMinimumSize(800,600);
    setMouseTracking(true);
}

void ShisenWidget::paintEvent(QPaintEvent *event) {
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

void ShisenWidget::drawTiles(QPainter& painter) const {
    int tileWidth = 54;
    int tileHeight = 65;
    int topCornerX = (width() - tileWidth*game.getCols())/2;
    int topCornerY = (height() - tileHeight*game.getRows())/2;

    //drawing tiles
    for(int i=0; i<game.getCols(); i++) {
        for(int j=game.getRows()-1; j>=0; j--) {
            std::vector<std::vector<const Tile*>> tiles = game.getTiles();
            Tile tile = *tiles[i][j];

            int targetX = topCornerX + i*tileWidth;
            int targetY = topCornerY + j*tileHeight;
            QRectF target(targetX, targetY, Tile::spriteWidth(), Tile::spriteHeight());

            QString selectedPath = ":/images/selected_tiles.png";
            QString normalPath = ":/images/mahjong_tiles.png";
            QString tilesheetPath = tile.isSelected() ? selectedPath : normalPath;

            QImage spriteSheet = QImage(tilesheetPath);
            QRectF source(tile.getX(), tile.getY(), Tile::spriteWidth(), Tile::spriteHeight());

            painter.drawImage(target, spriteSheet, source);
        }
    }
}

void ShisenWidget::redrawTile(QPainter& painter, const struct Space& space) {
    int tileWidth = 54;
    int tileHeight = 65;
    int verticalShift = Tile::spriteHeight() - tileHeight;
    int topCornerX = (width() - tileWidth*game.getCols())/2;
    int topCornerY = (height() - tileHeight*game.getRows())/2 + verticalShift;

    int targetX = topCornerX + tileWidth * space.col;
    int targetY = topCornerY + tileHeight * space.row;
    QRect target = QRect(targetX, targetY, tileWidth, tileHeight);

    std::vector<std::vector<const Tile*>> tiles = game.getTiles();
    const Tile& tile = *tiles[space.col][space.row];
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

bool ShisenWidget::gridContainsSpace(const struct Space &space) const {
    int row = space.row;
    int col = space.col;
    return  0 <= row && row < game.getRows() && 0 <= col && col < game.getCols();
}

struct Space ShisenWidget::findSpace(const unsigned x, const unsigned y) const {
    int tileWidth = 54;
    int tileHeight = 65;
    int topCornerX = (width() - tileWidth*12)/2;
    int topCornerY = (height() - tileHeight*5)/2 + Tile::spriteHeight() - tileHeight;

    int col = floor((double)(x - topCornerX) / tileWidth);
    int row = floor((double)(y - topCornerY) / tileHeight);
    struct Space space = {col, row};

    return space;
}

void ShisenWidget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();

    struct Space newHoveredSpace = findSpace(x,y);

    //mousing hovering over new tile
    if(gridContainsSpace(newHoveredSpace) && hoveredSpace != newHoveredSpace) {
        std::vector<std::vector<const Tile*>> tiles = game.getTiles();

        //mouse left previous tile
        if(gridContainsSpace(hoveredSpace)) {
            const Tile& prevHoveredTile = *tiles[hoveredSpace.col][hoveredSpace.row];
            game.markNotHovered(hoveredSpace);
            updatedSpace = hoveredSpace;
        }

        hoveredSpace = newHoveredSpace;
        const Tile& hoveredTile = *tiles[hoveredSpace.col][hoveredSpace.row];
        game.markHovered(hoveredSpace);
        repaint();
    }
    //mouse went from tile to empty space
    else if(!gridContainsSpace(newHoveredSpace) && gridContainsSpace(hoveredSpace)) {
//        Tile& hoveredTile = *game.getTiles()[hoveredSpace.col][hoveredSpace.row];
//        hoveredTile.markNotHovered();
        game.markNotHovered(hoveredSpace);
        updatedSpace = hoveredSpace;
        hoveredSpace = newHoveredSpace;
        repaint();
    }
}

void ShisenWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    struct Space clickedSpace = findSpace(x, y);

    //checking if tile needs to be updated
    if(gridContainsSpace(clickedSpace)) {
        std::vector<std::vector<const Tile*>> tiles = game.getTiles();
        const Tile& clickedTile = *tiles[clickedSpace.col][clickedSpace.row];

        //deselecting a tile
        if(clickedTile.isSelected()) {
            game.deselectTile(clickedSpace);
            updatedSpace = clickedSpace;
            repaint();
        }
        //selecting tile if it creates no more than 2 selected tiles total
        else if(gridContainsSpace(clickedSpace) && game.getSelectedTiles().size() < 2) {
            game.selectTile(clickedSpace);
            updatedSpace = clickedSpace;
            repaint();
        }
    }
}

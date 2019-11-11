#include "shisenwidget.h"
#include <QPainter>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

unsigned ShisenWidget::tileWidth = 54;
unsigned ShisenWidget::tileHeight= 65;

ShisenWidget::ShisenWidget(MainWindow* parent) : QWidget(parent) {
    game = Shisensho();
    drawBackground = true;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};
    path = std::vector<struct Space>();

    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(800,600);
    setMouseTracking(true);

    gridX = (width() - tileWidth*game.getCols())/2;
    gridY = (height() - tileHeight*game.getRows())/2;

    game.createTiles();
}

ShisenWidget::ShisenWidget(const unsigned cols, const unsigned rows, MainWindow* parent) : QWidget(parent){
    assert((cols * rows) % 2 == 0);

    game = Shisensho(cols, rows);
    drawBackground = true;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};
    path = std::vector<struct Space>();

    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(800,600);
    setMouseTracking(true);

    gridX = (width() - tileWidth*game.getCols())/2;
    gridY = (height() - tileHeight*game.getRows())/2;

    game.createTiles();
}

void ShisenWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //checking if a tile needs to be repainted
    if(gridContainsSpace(updatedSpace)) {
        qDebug() << "updating tile at space {" << updatedSpace.col << ", " << updatedSpace.row << "}";
        redrawTile(painter, updatedSpace);
        updatedSpace = {-1, -1};
    }

    //checking if any newly hovered tiles
    if(gridContainsSpace(hoveredSpace)) {
        qDebug() << "drawing hovered tile";
        redrawTile(painter, hoveredSpace);
    }

    //drawing any paths
    if(path.size() > 0) {
        qDebug() << "drawing path";
        drawPath(painter);
        path.clear();

        //adding delay before tiles removed
        drawBackground = false;
        int milliseconds = 500;
        QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
    }

    //checking if background needs to be drawn
    if(drawBackground) {
        qDebug() << "Drawing tiles";
        QBrush brush = QBrush(Qt::darkGreen);
        painter.fillRect(0, 0, width(), height(), brush);
        drawTiles(painter);
        drawBackground = false;
    }
}

void ShisenWidget::drawTiles(QPainter& painter) const {
    std::vector<std::vector<const Tile*>> tiles = game.getTiles();

    //drawing tiles
    for(int i=0; i<game.getCols(); i++) {
        for(int j=game.getRows()-1; j>=0; j--) {
            //space occupied by tile
            if(tiles[i][j] != nullptr) {
                Tile tile = *tiles[i][j];

                int targetX = gridX + i*tileWidth;
                int targetY = gridY + j*tileHeight;
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
}

void ShisenWidget::drawPath(QPainter& painter) const {
    assert(path.size() > 0);

    QPen pen = QPen(Qt::red, 4);
    painter.setPen(pen);

    for(int i=0; i<path.size()-1; i++) {
        struct Space space1 = path[i];
        struct Space space2 = path[i+1];

        QPoint point1 = findCenterPoint(space1);
        QPoint point2 = findCenterPoint(space2);

        painter.drawLine(point1, point2);
    }
}

void ShisenWidget::redrawTile(QPainter& painter, const struct Space& space) const {
    int verticalShift = Tile::spriteHeight() - tileHeight;
    int topCornerX = gridX;
    int topCornerY = gridY + verticalShift;

    int targetX = topCornerX + tileWidth * space.col;
    int targetY = topCornerY + tileHeight * space.row;
    QRect target = QRect(targetX, targetY, tileWidth, tileHeight);

    std::vector<std::vector<const Tile*>> tiles = game.getTiles();

    //checking if space contains a tile
    if(tiles[space.col][space.row] != nullptr) {
        Tile tile = *tiles[space.col][space.row];
        QString imagePath;

        //tile hovered over
        if(space == hoveredSpace && game.getSelectedTiles().size() < 2) {
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
}

bool ShisenWidget::gridContainsSpace(const struct Space &space) const {
    int row = space.row;
    int col = space.col;
    return  0 <= row && row < game.getRows() && 0 <= col && col < game.getCols();
}

struct Space ShisenWidget::findSpace(const unsigned x, const unsigned y) const {
    int verticalShift = Tile::spriteHeight() - tileHeight;
    int topCornerX = gridX;
    int topCornerY = gridY + verticalShift;

    int col = floor((double)(x - topCornerX) / tileWidth);
    int row = floor((double)(y - topCornerY) / tileHeight);
    struct Space space = {col, row};

    return space;
}

QPoint ShisenWidget::findCenterPoint(const struct Space& space) const {
    int col = space.col;
    int row = space.row;

    int verticalShift = Tile::spriteHeight() - tileHeight;
    int cornerX = gridX + col * tileWidth;
    int cornerY = gridY + row * tileHeight + verticalShift;

    int centerX = cornerX + tileWidth / 2;
    int centerY = cornerY + tileHeight / 2;

    return QPoint(centerX, centerY);
}

void ShisenWidget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();

    struct Space newHoveredSpace = findSpace(x,y);

    //mousing hovering over new tile
    if(!game.spaceEmpty(newHoveredSpace) && newHoveredSpace != hoveredSpace) {
        //mouse left previous tile
        if(gridContainsSpace(hoveredSpace) && !game.spaceEmpty(hoveredSpace)) {
            game.markNotHovered(hoveredSpace);
            updatedSpace = hoveredSpace;
        }

        hoveredSpace = newHoveredSpace;
        game.markHovered(hoveredSpace);
        update();
    }
    //mouse went from tile to empty space
    else if(game.spaceEmpty(newHoveredSpace) && !game.spaceEmpty(hoveredSpace)) {
        game.markNotHovered(hoveredSpace);
        updatedSpace = hoveredSpace;
        hoveredSpace = newHoveredSpace;
        update();
    }
}

void ShisenWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    struct Space clickedSpace = findSpace(x, y);

    //checking if tile needs to be updated
    if(!game.spaceEmpty(clickedSpace)) {
        std::vector<std::vector<const Tile*>> tiles = game.getTiles();
        const Tile& clickedTile = *tiles[clickedSpace.col][clickedSpace.row];

        //deselecting a tile
        if(clickedTile.isSelected()) {
            game.deselectTile(clickedSpace);
            updatedSpace = clickedSpace;
            update();
        }
        //selecting tile if less than 2 tiles already selected
        else if(game.getSelectedTiles().size() < 2) {
            game.selectTile(clickedSpace);
            updatedSpace = clickedSpace;
            repaint();

            //checking if the two selected tiles make a match
            if(game.getSelectedTiles().size() == 2) {
                std::vector<struct Space> selected = game.getSelectedTiles();
                struct Space space1 = selected[0];
                struct Space space2 = selected[1];
                std::vector<struct Space> path = game.findPath(space1, space2);

                const Tile tile1 = *tiles[space1.col][space1.row];
                const Tile tile2 = *tiles[space2.col][space2.row];

                //found match
                if(game.matchingTiles(tile1, tile2) && path.size() > 0) {
                    this->path = path;
                    game.removeSelectedTiles();
                    update();
                }
                //not a match
                else {
                    game.deselectTiles();
                    drawBackground = true;
                    update();
                }
            }
        }
    }
}

void ShisenWidget::redrawBackground() {
    drawBackground = true;
    update();
}

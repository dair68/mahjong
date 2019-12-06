#include "shisenwidget.h"
#include <QPainter>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QHBoxLayout>
#include <QInputDialog>
#include <sstream>

unsigned ShisenWidget::tileWidth = 54;
unsigned ShisenWidget::tileHeight= 65;

ShisenWidget::ShisenWidget(MainWindow* parent) : QWidget(parent), game(), gameStarted(false),
    drawBackground(false), updatedSpace({-1,-1}), hoveredSpace({-1, -1}), path() {

    //preventing window erasing to improving painting
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(800,600);
    setMouseTracking(true);

    winLabel = new QLabel("Game Over", this);
    winLabel->setStyleSheet("color: maroon;"
                            "background-color: rgba(255, 255, 255, 70%);"
                            "font: 50px Arial;");
    winLabel->setAlignment(Qt::AlignCenter);
    winLabel->setMinimumSize(width() * .4, height() * .3);
    winLabel->setMaximumSize(width() * .8, height() * .5);
    winLabel->hide();

    QLayout* layout = new QVBoxLayout();
    layout->addWidget(winLabel);
    layout->setAlignment(winLabel, Qt::AlignHCenter);
    setLayout(layout);

    QObject::connect(&game, &Shisensho::gameInitialized, this, &ShisenWidget::tilesFinished);
}

void ShisenWidget::startGame() {
    //getting game dimensions
    bool ok;
    QStringList options = {"Small (12x5)", "Medium (14x6)", "Large (18x8)"};
    QString gameDim = QInputDialog::getItem(this, "Game select", "Select game dimensions:",
                                                options, 0, false, &ok);

    qDebug() << gameDim;

    if (ok) {
        QStringList optionParts = gameDim.split(' ');
        QString dim = optionParts[1];
        qDebug() << dim;
        dim.chop(1);
        dim.remove(0, 1);

        QStringList dimensionChunks = dim.split('x');
        qDebug() << dimensionChunks;
        unsigned cols = dimensionChunks[0].toInt();
        unsigned rows = dimensionChunks[1].toInt();
        qDebug() << cols << rows;

        game = Shisensho(cols, rows);
        gridX = (width() - tileWidth*game.getCols())/2;
        gridY = (height() - tileHeight*game.getRows())/2;

        qDebug() << "dimensions: " << game.getCols() << game.getRows();
        game.createWinnableTiles();
    }
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

    //game over
    if(game.isOver()) {
        winLabel->show();
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

    //game still going
    if(!game.isOver()) {
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
}

void ShisenWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    struct Space clickedSpace = findSpace(x, y);

    //checking if tile needs to be updated
    if(!game.spaceEmpty(clickedSpace) && !game.isOver()) {
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
                std::list<struct Space> selected = game.getSelectedTiles();
                struct Space space1 = selected.front();
                struct Space space2 = selected.back();
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

void ShisenWidget::changeEvent(QEvent* event) {
    //window activation change event
    if(event->type() == QEvent::ActivationChange && gameStarted) {
        qDebug() << "change";
        redrawBackground();
    }
}

void ShisenWidget::redrawBackground() {
    qDebug() << "redrawing";
    drawBackground = true;
    update();
}

void ShisenWidget::tilesFinished() {
    qDebug() << "finished making tiles";
    gameStarted = true;
    redrawBackground();
}

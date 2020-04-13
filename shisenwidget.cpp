#include "shisenwidget.h"
#include <QPainter>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QHBoxLayout>
#include <QInputDialog>
#include <sstream>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>

unsigned ShisenWidget::tileWidth = 54;
unsigned ShisenWidget::tileHeight= 65;

ShisenWidget::ShisenWidget(MainWindow* parent) : QWidget(parent), game(),
    timeDisplay(this), timePenalty(0), gameStarted(false), drawBackground(false),
    updatedSpace({-1, -1}), hoveredSpace({-1, -1}), penaltySpace({-1, -1}), path(), gridX(0), gridY(0),
    undoButton("Undo", this), hintButton("Hint", this) {

//    QVBoxLayout* layout = new QVBoxLayout;
//    layout->addWidget(&time);
//    setLayout(layout);
    timeDisplay.setBackgroundColor(Qt::white);
    timeDisplay.setAlignment(Qt::AlignCenter);
    timeDisplay.resize(200, 100);
    //time.setBackgroundColor(Qt::darkGreen);

    //preventing window erasing to improving painting
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(1200,800);
    setMouseTracking(true);

    undoButton.setEnabled(false);
    hintButton.setEnabled(false);
    hintButton.move(500, 0);

    QObject::connect(&game, &Shisensho::gameInitialized, this, &ShisenWidget::startPainting);
    QObject::connect(&undoButton, &QPushButton::clicked, this, &ShisenWidget::undoButtonHandler);
    QObject::connect(&hintButton, &QPushButton::clicked, this, &ShisenWidget::markRemovableTiles);
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
        QString size = optionParts[0].toLower();
        QString filename = ":/levels/" + size + "_levels.txt";
        game.configRandomLevel(filename);

        gridX = (width() - tileWidth*game.getCols())/2;
        gridY = (height() - tileHeight*game.getRows())/2;
        hintButton.setEnabled(true);
        //time.start();
    }
}

void ShisenWidget::endGame() {
    timeDisplay.stop();
    undoButton.setEnabled(false);
    hintButton.setEnabled(false);
    QMessageBox gameOverBox;
    QString message = game.tilesLeft() ? "Game Over" : "Congratulations. You won!";
    gameOverBox.setText(message);

    QPushButton* retry = nullptr;

    //game lost
    if(game.tilesLeft()) {
        retry = gameOverBox.addButton("Try Again", QMessageBox::YesRole);
    }

    QPushButton* newGame = gameOverBox.addButton("New Game", QMessageBox::YesRole);
    QPushButton* quit = gameOverBox.addButton("Quit", QMessageBox::NoRole);
    gameOverBox.exec();

    //new game clicked
    if(gameOverBox.clickedButton() == newGame) {
        qDebug() << "New Game";
        reset();
        startGame();
    }
    //try again clicked
    else if(gameOverBox.clickedButton() == retry) {
        qDebug() << "Resetting tiles";
        reset();
        game.resetTiles();
        startPainting();
    }
    //quitting
    else {
        qDebug() << "Quit";
        QApplication::quit();
    }
}

void ShisenWidget::checkGameStatus() {
    //checking if game over
    if(gameStarted && game.isOver()) {
        qDebug() << "ending game";
        gameStarted = false;
        endGame();
    }
}

void ShisenWidget::reset() {
    game.clearTiles();
    gameStarted = false;
    drawBackground = false;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};
    path.clear();
    timeDisplay.reset();
    undoButton.setEnabled(false);
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
    if(gridContainsSpace(hoveredSpace) && timePenalty == 0) {
        qDebug() << "drawing hovered tile";
        redrawTile(painter, hoveredSpace);
    }
    //drawing time penalty
    if(timePenalty > 0) {
        drawTimePenalty(painter, penaltySpace);
        penaltySpace = {-1, -1};
        timePenalty = 0;
    }

    //drawing any paths
    if(path.size() > 0) {
        qDebug() << "drawing path";
        drawPath(painter);
        path.clear();

        //adding delay before tiles removed
        drawBackground = false;
        int milliseconds = 200;
        QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
    }

    //checking if background needs to be drawn
    if(drawBackground) {
        qDebug() << "Drawing tiles";
        QBrush brush = QBrush(Qt::darkGreen);
        painter.fillRect(0, 0, width(), height(), brush);
        drawTiles(painter);

        //delaying to allow painting to catch up, then checking if the game is over
        int milliseconds = 100;
        QTimer::singleShot(milliseconds, this, SLOT(checkGameStatus()));
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
                //qDebug() << tile.getId();

                int targetX = gridX + i*tileWidth;
                int targetY = gridY + j*tileHeight;
                QRectF target(targetX, targetY, Tile::spriteWidth(), Tile::spriteHeight());

                QString selectedPath = ":/images/selected_tiles.png";
                QString highlightedPath = ":/images/dark_tiles.png";
                QString normalPath = ":/images/mahjong_tiles.png";
                //QString tilesheetPath = tile.isSelected() ? selectedPath : normalPath;
                //qDebug() << tilesheetPath;
                QString tilesheetPath = tile.isHighlighted() ? highlightedPath : normalPath;

                QImage spriteSheet = QImage(tilesheetPath);
                QRectF source(tile.getX(), tile.getY(), Tile::spriteWidth(), Tile::spriteHeight());

                painter.drawImage(target, spriteSheet, source);
            }
        }
    }

    qDebug() << "tiles redrawn";
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
        QString hoverPath;
        QString normalPath;

        //selected tile
        if(tile.isSelected()) {
            hoverPath = ":/images/dim_selected_tiles.png";
            normalPath = ":/images/selected_tiles.png";
        }
        //highlighted tile
        else if(tile.isHighlighted()) {
            hoverPath = ":/images/darker_tiles.png";
            normalPath = ":/images/dark_tiles.png";
        }
        //unspecial tile
        else {
            hoverPath = ":/images/dim_tiles.png";
            normalPath = ":/images/mahjong_tiles.png";
        }

        QString imagePath = tile.isHoveredOver() ? hoverPath : normalPath;
        QImage spritesheet = QImage(imagePath);
        QRect source = QRect(tile.getX(), tile.getY() + verticalShift, tileWidth, tileHeight);
        painter.drawImage(target, spritesheet, source);
    }
}

void ShisenWidget::drawTimePenalty(QPainter& painter, const struct Space& space) const {
    int verticalShift = Tile::spriteHeight() - tileHeight;
    int topCornerX = gridX;
    int topCornerY = gridY + verticalShift;

    int width = 3*tileWidth;
    int height = tileHeight;
    int targetX = topCornerX + tileWidth * (space.col - 1);
    int targetY = topCornerY + tileHeight * space.row;
    QRect target = QRect(targetX, targetY, width, height);

    QString text = "+" + QString::number(timePenalty) + "s";
    painter.setPen(Qt::darkMagenta);
    painter.setFont(QFont("Times", 15, QFont::Bold));
    painter.drawText(target, Qt::AlignCenter, text);
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
                    game.unhighlightTiles();
                    game.removeSelectedTiles();
                    undoButton.setEnabled(true);
                    hintButton.setEnabled(true);
                    redrawBackground();
                }
                //not a match
                else {
                    penaltySpace = clickedSpace;
                    qDebug() << penaltySpace.col << penaltySpace.row;
                    timePenalty = 5;
                    timeDisplay.increaseTime(5);
                    repaint();
                    game.deselectTiles();
                    int milliseconds = 100;
                    QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
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

void ShisenWidget::startPainting() {
    qDebug() << "finished making tiles";
    gameStarted = true;
    redrawBackground();
    timeDisplay.start();
}

void ShisenWidget::undoButtonHandler() {
    game.undoLastMove();
    undoButton.setEnabled(false);
    redrawBackground();
}

void ShisenWidget::markRemovableTiles() {
    qDebug() << "clicked hint";
    game.markRemovableTiles();
    timePenalty = 60;
    timeDisplay.increaseTime(timePenalty);
    penaltySpace = {(int)game.getCols() / 2 - 1, -2};
    repaint();
    int milliseconds = 100;
    QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
    hintButton.setEnabled(false);
}

#include "shisenwidget.h"
#include <QPainter>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QSound>

QSize ShisenWidget::tileDimension = QSize(54, 65);

ShisenWidget::ShisenWidget(QWidget* parent) : QWidget(parent), game(),
    timeDisplay(this), timePenalty(0), gameStarted(false), drawBackground(false),
    tilesDeleted(false), updatedSpace({-1, -1}), hoveredSpace({-1, -1}), penaltyRect(), path(),
    undoSpaces(), gridCorner(), undoButton("Undo", this), hintButton("Hint", this), menu(this) {

    initializeAttributes();
    initializeTimeDisplay();
    initializeButtons();
    connectWidgets();
    redrawBackground();
}

void ShisenWidget::startNewGame() {
    reset();
    showNewGameDialog();
}

void ShisenWidget::showNewGameDialog() {
    //getting game dimensions
    bool ok;
    const QStringList options = {"Small (12x5)", "Medium (14x6)", "Large (18x8)"};
    const QString gameDim = QInputDialog::getItem(this, "Game select", "Select game dimensions:",
                                                options, 0, false, &ok);

    qDebug() << gameDim;

    //game size selected
    if (ok) {
        const QStringList optionParts = gameDim.split(' ');
        const QString size = optionParts[0].toLower();
        const QString filename = ":/levels/" + size + "_levels.txt";
        game.configRandomLevel(filename);

        gridCorner.setX((width() - tileDimension.width()*game.getCols())/2);
        gridCorner.setY((height() - tileDimension.height()*game.getRows())/2);
        hintButton.setEnabled(true);
        emit gameHasBegun();
    }
    //cancel button clicked. emitting title screen signal
    else {
        emit returnToTitle();
    }
}

void ShisenWidget::endGame() {
    timeDisplay.stop();
    undoButton.setEnabled(false);
    hintButton.setEnabled(false);
}

void ShisenWidget::showGameOverDialog() {
    const QString timeText = "Time: " + timeDisplay.getTime();
    const QString text = game.tilesLeft() ? "Game Over" : timeText + "\nCongratulations. You won!";
    QMessageBox gameOverBox;
    gameOverBox.setText(text);
    QPushButton* retry = nullptr;
    QSound winJingle(":/sounds/472707__neolein__guzheng-solo-trimmed.wav");
    QSound loseJingle(":/sounds/209917__veiler__gong-sabi-2.wav");

    //game lost
    if(game.tilesLeft()) {
        retry = gameOverBox.addButton("Try Again", QMessageBox::YesRole);
        loseJingle.play();
    }
    else {
        winJingle.play();
    }

    QPushButton* newGame = gameOverBox.addButton("New Game", QMessageBox::YesRole);
    QPushButton* quit = gameOverBox.addButton("Return To Title", QMessageBox::NoRole);
    gameOverBox.exec();
    //new game clicked
    if(gameOverBox.clickedButton() == newGame) {
        qDebug() << "New Game";
        loseJingle.stop();
        startNewGame();
    }
    //try again clicked
    else if(gameOverBox.clickedButton() == retry) {
        loseJingle.stop();
        restartGame();
    }
    //quitting
    else {
//        qDebug() << "Quit";
//        QApplication::quit();
        loseJingle.stop();
        emit returnToTitle();
    }
}

void ShisenWidget::checkGameStatus() {
    //checking if game over
    if(gameStarted && game.isOver()) {
        qDebug() << "ending game";
        gameStarted = false;
        endGame();
        emit gameOver();
        showGameOverDialog();
    }
}

void ShisenWidget::reset() {
    game.clearTiles();
    gameStarted = false;
    updatedSpace = {-1, -1};
    hoveredSpace = {-1, -1};
    path.clear();
    timeDisplay.reset();
    undoButton.setEnabled(false);
    hintButton.setEnabled(false);
}

void ShisenWidget::restartGame() {
    qDebug() << "Resetting tiles";
    reset();
    game.resetTiles();
    startPainting();
    emit gameHasBegun();
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
        drawTimePenalty(painter, penaltyRect);
        penaltyRect = QRect();
        timePenalty = 0;
    }

    //drawing any paths
    if(path.size() > 0) {
        qDebug() << "drawing path";
        drawPath(painter);

        //adding delay before tiles removed
        const int milliseconds = 100;
        QTimer::singleShot(milliseconds, this, SLOT(eraseTiles()));
    }

    //checking if a pair of tiles has been removed
    if(tilesDeleted) {
        //path to remove
        if(path.size() > 0) {
            erasePath(painter);
        }
        std::vector<struct Space> recentSpaces = game.getRecentSpaces();

        //redrawing each space
        for(struct Space space : recentSpaces) {
            redrawSpaceArea(painter, space);
        }
        tilesDeleted = false;
        emit deletedTilesDrawn();
    }

    //pair of tiles restored through undo
    if(undoSpaces.size() > 0) {
        for(struct Space space : undoSpaces) {
            redrawSpaceArea(painter, space);
        }
        emit tilesRestored();
    }

    //checking if background needs to be drawn
    if(drawBackground) {
        qDebug() << "Drawing tiles";
        const QBrush brush = QBrush(Qt::darkGreen);
        painter.fillRect(0, 0, width(), height(), brush);

        //drawing tiles if game started
        if(gameStarted) {
            drawTiles(painter);
        }

        //delaying to allow painting to catch up, then checking if the game is over
        const int milliseconds = 100;
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
                const Tile tile = *tiles[i][j];
                //qDebug() << tile.getId();

                const int targetX = gridCorner.x() + i*tileDimension.width();
                const int targetY = gridCorner.y() + j*tileDimension.height();
                const QRectF target(targetX, targetY, Tile::spriteWidth(), Tile::spriteHeight());

                const QString selectedPath = ":/images/selected_tiles.png";
                const QString highlightedPath = ":/images/dark_tiles.png";
                const QString normalPath = ":/images/mahjong_tiles.png";
                //QString tilesheetPath = tile.isSelected() ? selectedPath : normalPath;
                //qDebug() << tilesheetPath;
                const QString tilesheetPath = tile.isHighlighted() ? highlightedPath : normalPath;

                const QImage spriteSheet = QImage(tilesheetPath);
                const QRectF source(tile.getX(), tile.getY(), Tile::spriteWidth(), Tile::spriteHeight());

                painter.drawImage(target, spriteSheet, source);
            }
        }
    }

    qDebug() << "tiles redrawn";
}

void ShisenWidget::drawPath(QPainter& painter) const {
    assert(path.size() > 0);

    const QPen pen = QPen(Qt::red, 4);
    painter.setPen(pen);

    //drawing each path segment
    for(int i=0; i<path.size()-1; i++) {
        const struct Space space1 = path[i];
        const struct Space space2 = path[i+1];

        const QPoint point1 = findCenterPoint(space1);
        const QPoint point2 = findCenterPoint(space2);

        painter.drawLine(point1, point2);
    }
}

void ShisenWidget::erasePath(QPainter& painter) const {
    assert(path.size() > 0);

    //erasing each path segment
    for(int i=0; i<path.size()-1; i++) {
        const struct Space space1 = path[i];
        const struct Space space2 = path[i+1];

        const QPoint point1 = findCenterPoint(space1);
        const QPoint point2 = findCenterPoint(space2);

        int left;
        int top;
        int width;
        int height;

        //points vertically connected
        if(point1.x() == point2.x()) {
            left = point1.x() - 5;
            top = std::min(point1.y() - 5, point2.y() - 5);
            width = 10;
            height = std::abs(point1.y() - point2.y()) + 10;
        }
        //points horizontally connected
        else {
            left = std::min(point1.x() - 5, point2.x() - 5);
            top = point1.y() - 5;
            width = std::abs(point1.x() - point2.x()) + 10;
            height = 10;
        }

        painter.fillRect(QRect(left, top, width, height), QBrush(Qt::darkGreen));
    }
    emit pathErasable();
}

void ShisenWidget::redrawSpaceArea(QPainter& painter, const struct Space& space) const {
    const QBrush brush = QBrush(Qt::darkGreen);
    const QRect rect = findBoundingRect(space);
    const int tileWidth = tileDimension.width();
    const int tileHeight = tileDimension.height();
    const int paddingH = tileWidth / 2;
    const int paddingV = tileHeight / 2;
    painter.fillRect(rect.x() - paddingH, rect.y() - paddingV, 2*tileWidth, 2*tileHeight, brush);

    //drawing tiles in spaces
    for(int i=1; i >= -1; i--) {
        for(int j=-1; j <= 1; j++) {
            const struct Space paintedSpace = {space.col + j, space.row + i};
            const Tile* tile = nullptr;

            //space within tile grid
            if(game.gridContainsSpace(paintedSpace)) {
                tile = game.getTiles()[paintedSpace.col][paintedSpace.row];
            }

            //tile at location
            if(tile != nullptr) {
                const QRect paintedRect = findBoundingRect(paintedSpace);
                const int leftEdge = paintedRect.x();
                int topEdge;
                const int targetWidth = (j == 1) ? tileWidth : Tile::spriteWidth();
                int targetHeight;

                //topmost tiles only drawn as high as tile face
                if(i == -1) {
                    topEdge = paintedRect.y();
                    targetHeight = tileHeight;
                }
                else {
                    topEdge = paintedRect.y() - Tile::spriteHeight() + tileHeight;
                    targetHeight = Tile::spriteHeight();
                }

                const QRect targetRect (leftEdge, topEdge, targetWidth, targetHeight);
                const QString imagePath = ":/images/mahjong_tiles.png";
                QRect sourceRect;

                //only tile face redrawn
                if(i == -1) {
                    const int y = tile->getY() + Tile::spriteHeight() - tileHeight;
                    sourceRect = QRect(tile->getX(), y, targetWidth, targetHeight);
                }
                else {
                    sourceRect = QRect(tile->getX(), tile->getY(), targetWidth, targetHeight);
                }

                painter.drawImage(targetRect, QImage(imagePath), sourceRect);
            }
        }
    }
}

void ShisenWidget::redrawTile(QPainter& painter, const struct Space& space) const {
    const int verticalShift = Tile::spriteHeight() - tileDimension.height();
    const int topCornerX = gridCorner.x();
    const int topCornerY = gridCorner.y() + verticalShift;

    const int targetX = topCornerX + tileDimension.width() * space.col;
    const int targetY = topCornerY + tileDimension.height() * space.row;
    const QRect target = QRect(targetX, targetY, tileDimension.width(), tileDimension.height());

    const std::vector<std::vector<const Tile*>> tiles = game.getTiles();

    //checking if space contains a tile
    if(tiles[space.col][space.row] != nullptr) {
        const Tile tile = *tiles[space.col][space.row];
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

        const QString imagePath = tile.isHoveredOver() ? hoverPath : normalPath;
        const QImage spritesheet = QImage(imagePath);
        const QRect source = QRect(QPoint(tile.getX(), tile.getY() + verticalShift), tileDimension);
        painter.drawImage(target, spritesheet, source);
    }
}

void ShisenWidget::drawTimePenalty(QPainter& painter, const QRect& rect) const {
    const QString text = "+" + QString::number(timePenalty) + "s";
    painter.setPen(Qt::darkMagenta);
    painter.setFont(QFont("Times", 15, QFont::Bold));
    painter.drawText(rect, Qt::AlignCenter, text);
}

bool ShisenWidget::gridContainsSpace(const struct Space &space) const {
    const int row = space.row;
    const int col = space.col;
    return  0 <= row && row < game.getRows() && 0 <= col && col < game.getCols();
}

struct Space ShisenWidget::findSpace(const unsigned& x, const unsigned& y) const {
    const int verticalShift = Tile::spriteHeight() - tileDimension.height();
    const int topCornerX = gridCorner.x();
    const int topCornerY = gridCorner.y() + verticalShift;

    const int col = floor((double)(x - topCornerX) / tileDimension.width());
    const int row = floor((double)(y - topCornerY) / tileDimension.height());
    const struct Space space = {col, row};

    return space;
}

QPoint ShisenWidget::findCenterPoint(const struct Space& space) const {
    const QRect boundingRect = findBoundingRect(space);
    const int centerX = boundingRect.x() + tileDimension.width() / 2;
    const int centerY = boundingRect.y() + tileDimension.height() / 2;
    return QPoint(centerX, centerY);
}

QRect ShisenWidget::findBoundingRect(const struct Space& space) const {
    const int verticalShift = Tile::spriteHeight() - tileDimension.height();
    const int rectX = gridCorner.x()+ space.col * tileDimension.width();
    const int rectY = gridCorner.y() + space.row * tileDimension.height() + verticalShift;
    return QRect(rectX, rectY, tileDimension.width(), tileDimension.height());
}

void ShisenWidget::mouseMoveEvent(QMouseEvent* event) {
    const int x = event->x();
    const int y = event->y();

    //game still going
    if(!game.isOver()) {
        const struct Space newHoveredSpace = findSpace(x,y);

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
    const int x = event->x();
    const int y = event->y();

    const struct Space clickedSpace = findSpace(x, y);

    //checking if tile needs to be updated
    if(!game.spaceEmpty(clickedSpace) && !game.isOver()) {
        QSound::play(":/sounds/478197__jonnyruss01__click-1.wav");
        const std::vector<std::vector<const Tile*>> tiles = game.getTiles();
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
                const std::list<struct Space> selected = game.getSelectedTiles();
                const struct Space space1 = selected.front();
                const struct Space space2 = selected.back();
                const std::vector<struct Space> path = game.findPath(space1, space2);

                const Tile tile1 = *tiles[space1.col][space1.row];
                const Tile tile2 = *tiles[space2.col][space2.row];

                //found match
                if(game.matchingTiles(tile1, tile2) && path.size() > 0) {
                    this->path = path;
                    game.removeSelectedTiles();
                    undoButton.setEnabled(true);

                    //tiles matched with hint; setting tiles back to normal
                    if(!hintButton.isEnabled()) {
                        qDebug() << "unhighligting tiles";
                        game.unhighlightTiles();
                        hintButton.setEnabled(true);
                        repaint();

                        const int milliseconds = 10;
                        QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
                        //redrawBackground();
                    }
                    else {
                        repaint();
                    }
                    //redrawBackground();
                }
                //not a match
                else {
                    penaltyRect = findBoundingRect(clickedSpace);
                    timePenalty = 5;
                    timeDisplay.increaseTime(5);
                    repaint();
                    game.deselectTiles();
                    const int milliseconds = 100;
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

void ShisenWidget::eraseTiles() {
    tilesDeleted = true;
    repaint();
}

void ShisenWidget::startPainting() {
    qDebug() << "finished making tiles";
    gameStarted = true;
    redrawBackground();
    timeDisplay.start();
    hintButton.setEnabled(true);
}

void ShisenWidget::undoButtonHandler() {
    undoSpaces = game.getRecentSpaces();
    game.undoLastMove();
    undoButton.setEnabled(false);
    repaint();
}

void ShisenWidget::markRemovableTiles() {
    qDebug() << "clicked hint";
    game.markRemovableTiles();
    timePenalty = 60;
    timeDisplay.increaseTime(timePenalty);
    penaltyRect = QRect(hintButton.x(), hintButton.y() - 60, hintButton.width(), tileDimension.height());
    repaint();
    const int milliseconds = 100;
    QTimer::singleShot(milliseconds, this, SLOT(redrawBackground()));
    hintButton.setEnabled(false);
}

void ShisenWidget::clearPath() {
    path.clear();
}

void ShisenWidget::clearUndoSpaces() {
    undoSpaces.clear();
}

void ShisenWidget::initializeAttributes() {
    //preventing window erasing to improving painting
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(1200,900);
    setMouseTracking(true);
}

void ShisenWidget::initializeTimeDisplay() {
    timeDisplay.setAlignment(Qt::AlignCenter);
    timeDisplay.resize(200, 100);
    timeDisplay.move(width() / 2 - timeDisplay.width() / 2, 10);
    timeDisplay.setStyleSheet("color: white; background-color: green; font: 12pt");
}

void ShisenWidget::initializeButtons() {
    const int hMargin = 10;
    const int vMargin = 60;
    undoButton.move(width() / 2 - undoButton.width() - hMargin, height() - vMargin);
    hintButton.move(width() / 2 + hMargin, height() - vMargin);

    undoButton.setEnabled(false);
    hintButton.setEnabled(false);
    undoButton.setStyleSheet("font: 12pt");
    hintButton.setStyleSheet("font: 12pt");
}

void ShisenWidget::connectWidgets() {
    connect(&game, SIGNAL(gameInitialized()), this, SLOT(startPainting()));
    connect(&undoButton, SIGNAL(clicked()), this, SLOT(undoButtonHandler()));
    connect(&hintButton, SIGNAL(clicked()), this, SLOT(markRemovableTiles()));
    connect(this, SIGNAL(pathErasable()), this, SLOT(clearPath()));
    //connect(this, SIGNAL(deletedTilesDrawn()), &game, SLOT(clearRecentSpaces()));
    connect(this, SIGNAL(tilesRestored()), this, SLOT(clearUndoSpaces()));
    connect(&menu, SIGNAL(modalCreated()), &timeDisplay, SLOT(stop()));
    connect(&menu, SIGNAL(noSelected()), &timeDisplay, SLOT(start()));
    connect(&menu, SIGNAL(helpSelected()), this, SIGNAL(showHelp()));
    connect(&menu, SIGNAL(returnToTitleSelected()), this, SIGNAL(returnToTitle()));
    connect(&menu, SIGNAL(restartSelected()), this, SLOT(restartGame()));
    connect(&menu, SIGNAL(newGameSelected()), this, SLOT(startNewGame()));
    connect(&menu, SIGNAL(modalCreated()), this, SIGNAL(gamePaused()));
    connect(&menu, SIGNAL(noSelected()), this, SIGNAL(gameResumed()));
}

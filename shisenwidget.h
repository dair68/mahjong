#ifndef SHISENWIDGET_H
#define SHISENWIDGET_H

#include <QWidget>
#include <QLabel>
#include "mainwindow.h"
#include "tile.h"
#include "shisensho.h"

class ShisenWidget : public QWidget {
    Q_OBJECT
public:
    //creates shisensho screen
    //@param parent - a parent MainWindow. nullptr by default
    ShisenWidget(MainWindow* parent=nullptr);

    //asks player for a game of custom size, then starts game
    void startGame();

    //paints screen based on state of the game
    void paintEvent(QPaintEvent* event) override;

    //draws all the tiles at once
    //@param painter - a QPainter object pointing to this widget
    void drawTiles(QPainter& painter) const;

    //draws path over a pair of matching tiles before removing them
    //@param painter - a QPainter object pointing to this widget
    void drawPath(QPainter& painter) const;

    //redraws a single space in grid
    //@param painter - a QPainter object pointing to this widget
    //@param space - space containing tile to be redrawn
    void redrawTile(QPainter& painter, const struct Space& space) const;

    //checks if a given space exists in tile grid numbered from 0
    //@param space - some space with a column and row
    bool gridContainsSpace(const struct Space& space) const;

    //finds the space that contains the given pixel
    //@param x - x coordinate of pixel. 0 is left side of screen.
    //@param y - y coordinate of pixel. 0 is top edge of screen.
    //returns space containing pixel. space might lie outside the grid
    struct Space findSpace(const unsigned x, const unsigned y) const;

    //finds the point at tile center for a given space
    //@param space - some space. does not need to be in grid or contain tile
    //returns the point that would be in the center of tile located at space
    QPoint findCenterPoint(const struct Space& space) const;

    //colors tile if mouse hovers over it
    void mouseMoveEvent(QMouseEvent* event) override;

    //event for clicking tiles
    void mousePressEvent(QMouseEvent* event) override;

    //event for window state changing
    void changeEvent(QEvent* event) override;
public slots:
    //sets drawBackground to true
    void redrawBackground();

    //prints that game has been initialized to console
    void tilesFinished();
private:
    Shisensho game;
    bool gameStarted;
    bool drawBackground;
    struct Space updatedSpace;
    struct Space hoveredSpace;
    std::vector<struct Space> path;
    int gridX;
    int gridY;
    static unsigned tileWidth;
    static unsigned tileHeight;
    QLabel* winLabel;
};

#endif // SHISENWIDGET_H

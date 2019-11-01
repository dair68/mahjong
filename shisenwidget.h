#ifndef SHISENWIDGET_H
#define SHISENWIDGET_H

#include <QWidget>
#include "mainwindow.h"
#include "tile.h"
#include "shisensho.h"

class ShisenWidget : public QWidget {
public:
    //constructor
    //@param parent - a parent MainWindow. nullptr by default.
    ShisenWidget(MainWindow* parent=nullptr);

    //paints screen based on state of the game
    void paintEvent(QPaintEvent* event) override;

    //draws all the tiles at once
    //@param painter - a QPainter object pointing to this widget
    void drawTiles(QPainter& painter) const;

    //redraws a single space in grid
    //@param painter - a QPainter object pointing to this widget
    //@param space - space containing tile to be redrawn
    void redrawTile(QPainter& painter, const struct Space& space);

    //checks if a given space exists in tile grid numbered from 0
    //@param space - some space with a column and row
    bool gridContainsSpace(const struct Space& space) const;

    //finds the space that contains the given pixel
    //@param x - x coordinate of pixel. 0 is left side of screen.
    //@param y - y coordinate of pixel. 0 is top edge of screen.
    //returns space containing pixel. space might lie outside the grid
    struct Space findSpace(const unsigned x, const unsigned y) const;

    //colors tile if mouse hovers over it
    void mouseMoveEvent(QMouseEvent* event) override;

    //event for clicking tiles
    void mousePressEvent(QMouseEvent* event) override;
private:
    Shisensho game;
    bool tilesDrawn;
    bool backgroundDrawn;
    struct Space updatedSpace;
    struct Space hoveredSpace;
};

#endif // SHISENWIDGET_H

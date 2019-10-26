#ifndef SHISENSHO_H
#define SHISENSHO_H

#include <QWidget>
#include "mainwindow.h"
#include "tile.h"

struct Space {
    int col;
    int row;
};

//checks if two spaces have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator==(const struct Space& spaceLeft, const struct Space& spaceRight);

//checks if two spaces do not have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator!=(const struct Space& spaceLeft, const struct Space& spaceRight);

class Shisensho : public QWidget {
public:
    //constructor
    //@param parent - a parent MainWindow. nullptr by default.
    Shisensho(MainWindow* parent=nullptr);

    //paints screen based on state of the game
    void paintEvent(QPaintEvent *event) override;

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

    //finds the grid space that contains the given pixel
    //@param x - x coordinate of pixel. 0 is left side of screen.
    //@param y - y coordinate of pixel. 0 is top edge of screen.
    //returns space with (col, row) = (-1, -1) if pixel not in grid, returns space otherwise
    struct Space findSpace(const unsigned x, const unsigned y) const;

    //colors tile if mouse hovers over it
    void mouseMoveEvent(QMouseEvent* event) override;

    //event for clicking tiles
    void mousePressEvent(QMouseEvent* event) override;
private:
    unsigned cols;
    unsigned rows;
    std::vector<std::vector<Tile>> tiles;
    bool tilesDrawn;
    bool backgroundDrawn;
    struct Space updatedSpace;
    struct Space hoveredSpace;
};

#endif // SHISENSHO_H

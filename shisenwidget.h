#ifndef SHISENWIDGET_H
#define SHISENWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include "mainwindow.h"
#include "tile.h"
#include "shisensho.h"
#include "stopwatch.h"

class ShisenWidget : public QWidget {
    Q_OBJECT
public:
    //creates shisensho screen
    //@param parent - a parent MainWindow. nullptr by default
    ShisenWidget(MainWindow* parent=nullptr);

    //asks player for a game of custom size, then starts game
    void startGame();

    //shows game over dialog and acts accordingly
    void endGame();

    //resets widget fields
    void reset();

    //paints screen based on state of the game
    void paintEvent(QPaintEvent* event) override;

    //draws all the tiles at once. will draw tiles as highlighted if needed.
    //@param painter - a QPainter object pointing to this widget
    void drawTiles(QPainter& painter) const;

    //draws path over a pair of matching tiles before removing them
    //@param painter - a QPainter object pointing to this widget
    void drawPath(QPainter& painter) const;

    //redraws a single space in grid. will draw penalty over space if necessary.
    //@param painter - a QPainter object pointing to this widget
    //@param space - space containing tile to be redrawn
    void redrawTile(QPainter& painter, const struct Space& space) const;

    //draws the time penalty over a certain space in grid
    //@param painter - a QPainter object pointing to this widget
    //@param rect - rectangle where penalty text will be drawn
    void drawTimePenalty(QPainter& painter, const QRect& rect) const;

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

    //finds the rectangle bounding the face of a tile at a given space
    //@param space - some space. does not need to be in grid or contain tile
    QRect findBoundingRect(const struct Space& space) const;

    //colors tile if mouse hovers over it
    void mouseMoveEvent(QMouseEvent* event) override;

    //event for clicking tiles
    void mousePressEvent(QMouseEvent* event) override;

    //event for window state changing
    void changeEvent(QEvent* event) override;

public slots:
    //sets drawBackground to true
    void redrawBackground();

    //starts the paint events and ticking of time display
    void startPainting();

    //checks if the game is over and ends things accordingly if so
    void checkGameStatus();

    //restores last 2 tiles if undo mode, removes last 2 tiles if redo mode
    void undoButtonHandler();

    //highlights the currently removable tiles and incurs 1 minute time penalty
    void markRemovableTiles();

    //
private:
    //sets initial attributes for the widget.
    void initializeAttributes();

    //sets initial attributes for the time display
    void initializeTimeDisplay();

    //sets initial attributes for the buttons
    void initializeButtons();

    //creates the menu
    void createMenu();

    Shisensho game;
    Stopwatch timeDisplay;
    unsigned timePenalty;
    bool gameStarted;
    bool drawBackground;
    struct Space updatedSpace;
    struct Space hoveredSpace;
    QRect penaltyRect;
    std::vector<struct Space> path;
    int gridX;
    int gridY;
    static unsigned tileWidth;
    static unsigned tileHeight;
    QPushButton undoButton;
    QPushButton hintButton;
    QMenuBar menuBar;
};

#endif // SHISENWIDGET_H

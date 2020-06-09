#ifndef SHISENWIDGET_H
#define SHISENWIDGET_H

#include "tile.h"
#include "shisensho.h"
#include "stopwatch.h"
#include "shisenmenu.h"
#include <QWidget>
#include <QPushButton>

class ShisenWidget : public QWidget {
    Q_OBJECT
public:
    //constructor
    //@param parent - pointer to parent widget
    ShisenWidget(QWidget* parent=nullptr);

    //shows dialog asking for game dimensions. game begins promptly after selection
    void showNewGameDialog();

    //stops the game
    void endGame();

    //shows the dialog that will allow user to play new game or redo current game
    void showGameOverDialog();

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

    //erases the path from the widget. does not restore tiles erased in process.
    //@param painter - a QPainter object pointing to this widget
    void erasePath(QPainter& painter) const;

    //repaints a space and it's 8 surrounding spaces
    //@param painter - a painter pointing to this widget
    //@param space - space of interest
    void redrawSpaceArea(QPainter& painter, const struct Space& space) const;

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
    struct Space findSpace(const unsigned& x, const unsigned& y) const;

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

    //erases pair of tiles and removes any paths
    void eraseTiles();

    //starts the paint events and ticking of time display
    void startPainting();

    //checks if the game is over and ends things accordingly if so
    void checkGameStatus();

    //restarts current or previously played game
    void restartGame();

    //asks player for a game of custom size, then starts game. existing game removed.
    void startNewGame();

    //restores last 2 tiles removed, if possible
    void undoButtonHandler();

    //highlights the currently removable tiles and incurs 1 minute time penalty
    void markRemovableTiles();

    //clears current path stored
    void clearPath();

    //clears widget's undo spaces
    void clearUndoSpaces();

signals:
    //emit when ready to return to title screen
    void returnToTitle();

    //emit when help button in menu clicked
    void showHelp();

    //emit when a game of shisensho has begun
    void gameHasBegun();

    //emit when a modal dialog on screen
    void gamePaused();

    //emit when modal dialog closed without selecting options
    void gameResumed();

    //emit when game has ended
    void gameOver();

    //emit when path can be safely erased
    void pathErasable() const;

    //emit when deleted tiles finished drawing
    void deletedTilesDrawn();

    //emit when undo button restores tiles
    void tilesRestored();

private:
    //sets initial attributes for the widget.
    void initializeAttributes();

    //sets initial attributes for the time display
    void initializeTimeDisplay();

    //sets initial attributes for the buttons
    void initializeButtons();

    //connects all appropriate signals and slots
    void connectWidgets();

    Shisensho game;
    Stopwatch timeDisplay;
    unsigned timePenalty;
    bool gameStarted;
    bool drawBackground;
    bool tilesDeleted;
    struct Space updatedSpace;
    struct Space hoveredSpace;
    QRect penaltyRect;
    std::vector<struct Space> path;
    std::vector<struct Space> undoSpaces;
    QPoint gridCorner;
    static QSize tileDimension;
    QPushButton undoButton;
    QPushButton hintButton;
    ShisenMenu menu;
};

#endif // SHISENWIDGET_H

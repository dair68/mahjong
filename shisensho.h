#ifndef SHISENSHO_H
#define SHISENSHO_H

#include <QWidget>
#include "mainwindow.h"
#include "tile.h"

class Shisensho : public QWidget {
public:
    //constructor
    //@param parent - a parent MainWindow. nullptr by default.
    Shisensho(MainWindow* parent=nullptr);

    //paints screen based on state of the game
    void paintEvent(QPaintEvent *event) override;
private:
    unsigned cols;
    unsigned rows;
    std::vector<std::vector<Tile>> tiles;
};

#endif // SHISENSHO_H

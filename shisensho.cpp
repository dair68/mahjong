#include "shisensho.h"
#include <QPainter>
#include <time.h>


Shisensho::Shisensho(MainWindow* parent) : QWidget(parent) {
    setStyleSheet(
      "background-color: green;"
    );

    cols = 12;
    rows = 5;
    tiles = std::vector<std::vector<Tile>>(cols, std::vector<Tile>(rows));

    //setting seed for random tiles
    srand(time(NULL));

    //generating random tiles
    for(int i=0; i<cols; i++) {
        for(int j=0; j<rows; j++) {
            tiles[i][j] = randomTile();
        }
    }
}

void Shisensho::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    int tileWidth = 54;
    int tileHeight = 65;
    int topCornerX = (width() - tileWidth*12)/2;
    int topCornerY = (height() - tileHeight*5)/2;

    //drawing tiles
    for(int i=0; i<cols; i++) {
        for(int j=rows-1; j>=0; j--) {
            Tile tile = tiles[i][j];

            int targetX = topCornerX + i*tileWidth;
            int targetY = topCornerY + j*tileHeight;
            QRectF target(targetX, targetY, Tile::spriteWidth(), Tile::spriteHeight());

            tile.render(painter, target);
        }
    }
}

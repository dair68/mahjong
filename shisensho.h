#ifndef SHISENSHO_H
#define SHISENSHO_H

#include <QWidget>
#include "mainwindow.h"

class Shisensho : public QWidget {
public:
    //constructor
    //@param parent - a parent MainWindow. nullptr by default.
    Shisensho(MainWindow* parent=nullptr);
};

#endif // SHISENSHO_H

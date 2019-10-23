#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QWidget>
#include "mainwindow.h"

class TitleScreen : public QWidget
{
public:
    //creates title screen
    //@param parent - pointer to parent widget
    TitleScreen(MainWindow* parent=nullptr);
};

#endif // TITLESCREEN_H

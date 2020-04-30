#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "helpwidget.h"
#include "titlescreen.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //creates main window starting at title screen
    //@param parent - pointer to parent widget
    MainWindow(QWidget* parent=nullptr);

public slots:
    //changes window to shisensho screen
    void toShisensho();

    //changes central widget to title screen
    void toTitle();

signals:
    //set central widget to title screen
    void enteredTitleScreen();

private:
    HelpWidget help;
};

#endif // MAINWINDOW_H

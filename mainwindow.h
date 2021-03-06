#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "helpwidget.h"
#include "musicplayer.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    //creates main window starting at title screen
    //@param parent - pointer to parent widget
    MainWindow(QWidget* parent = nullptr);

public slots:
    //changes window to shisensho screen
    void toShisensho();

    //changes central widget to title screen
    void toTitle();

    //changes central widget to credits screen
    void toCredits();

signals:
    //emit central widget is shisen widget
    void enteredShisenScreen();

private:
    HelpWidget help;
    MusicPlayer player;
};

#endif // MAINWINDOW_H

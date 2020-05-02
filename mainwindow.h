#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "helpwidget.h"
#include "titlescreen.h"
#include "musicplayer.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    //creates main window starting at title screen
    //@param parent - pointer to parent widget
    MainWindow(QWidget* parent=nullptr);

public slots:
    //changes window to shisensho screen
    void toShisensho();

    //changes central widget to title screen along with music
    void toTitle();

signals:
    void enteredTitleScreen();

private:
    HelpWidget help;
    MusicPlayer player;
};

#endif // MAINWINDOW_H

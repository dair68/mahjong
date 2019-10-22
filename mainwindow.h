#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
public:
    //creates main window starting at title screen
    MainWindow();

    //creates a main window
    //@param central - pointer to the central widget of the window
    MainWindow(QWidget* central);
private:
    //initializes the dimensions and other attributes of window
    void initializeSettings();
};
#endif // MAINWINDOW_H

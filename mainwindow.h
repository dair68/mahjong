#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //creates main window starting at title screen
    //@param parent - pointer to parent widget
    MainWindow(QWidget* parent=nullptr);

    //creates a main window
    //@param central - pointer to the central widget of the window
    //@param parent - pointer to parent widget
    MainWindow(QWidget* central, QWidget* parent=nullptr);

public slots:
    //changes window to shisensho screen
    void toShisensho();

    //changes central widget to title screen
    void toTitle();

private:
    //initializes the dimensions and other attributes of window
    void initializeSettings();
};
#endif // MAINWINDOW_H

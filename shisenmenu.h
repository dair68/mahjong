#ifndef SHISENMENU_H
#define SHISENMENU_H

#include <QMenuBar>

class ShisenMenu : public QMenuBar
{
    Q_OBJECT
public:
    //constructor
    //@parent - pointer to parent widget
    ShisenMenu(QWidget* parent = nullptr);

public slots:
    //shows dialog asking if current game should be restarted
    void showRestartDialog() const;

    //shows dialog asking to start a new game
    void showNewGameDialog() const;

    //shows dialog asking user to quit program
    void showQuitDialog() const;

    //shows dialog asking user to return to title screen
    void showTitleDialog() const;

 signals:
    //emit when a menu creates a modal dialog
    void modalCreated() const;

    //emit when user clicks help option
    void helpSelected();

    //emit whenever a "No" option is selected from any dialogs created
    void noSelected() const;

    //emit when user selects to restart game
    void restartSelected() const;

    //emit when user selects new game
    void newGameSelected() const;

    //emit when user selects return to title screen
    void returnToTitleSelected() const;
};

#endif // SHISENMENU_H

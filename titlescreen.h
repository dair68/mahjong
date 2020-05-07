#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QWidget>

class TitleScreen : public QWidget
{
    Q_OBJECT
public:
    //creates title screen
    //@param parent - pointer to parent widget
    TitleScreen(QWidget* parent=nullptr);

    //overriding paint event for stylesheet purposes
    void paintEvent(QPaintEvent *event) override;

signals:
    //emit when play button clicked
    void playButtonClicked();

    //emit when tutorial button clicked
    void tutorialButtonClicked();

    //emit when credits button clicked
    void creditsButtonClicked();
};

#endif // TITLESCREEN_H

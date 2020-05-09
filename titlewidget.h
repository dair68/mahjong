#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    //constructor
    //@param parent - pointer to parent widget
    TitleWidget(QWidget* parent = nullptr);

    //overriding paint event for stylesheet purposes
    void paintEvent(QPaintEvent *event) override;

public slots:
    //shows the quit dialog
    void showQuitDialog();

signals:
    //emit when play button clicked
    void playButtonClicked();

    //emit when tutorial button clicked
    void tutorialButtonClicked();

    //emit when credits button clicked
    void creditsButtonClicked();
};

#endif // TITLESCREEN_H

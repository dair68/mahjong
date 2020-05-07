#ifndef CREDITSWIDGET_H
#define CREDITSWIDGET_H

#include <QWidget>
#include <QFrame>


class CreditsWidget : public QWidget {
    Q_OBJECT
public:
    //creates widget
    //@param parent - pointer to parent widget
    CreditsWidget(QWidget* parent = nullptr);

    //overriding paint event for background image
    void paintEvent(QPaintEvent *event) override;

signals:
    //emit when back to title button clicked
    void backButtonClicked();
};

#endif // CREDITSWIDGET_H

#ifndef CREDITSWIDGET_H
#define CREDITSWIDGET_H

#include <QWidget>
#include <QLabel>

class CreditsWidget : public QWidget {
    Q_OBJECT
public:
    //constructor
    //@param parent - pointer to parent widget
    CreditsWidget(QWidget* parent = nullptr);

    //overriding paint event to display background image
    void paintEvent(QPaintEvent* event) override;

signals:
    //emit when back to title button clicked
    void backButtonClicked();

private:
    //creates and positions credit text
    void createCreditLabels();

    //creates image credits
    //returns pointer to label with credit text and links
    QLabel* createImageCredits();

    //create music credits
    //returns pointer to label with credit text and links
    QLabel* createMusicCredits();

    //creates sound credits
    //returns pointer to label with credit text and links
    QLabel* createSoundCredits();

    //creates programming credits
    //returns pointer to label with credit text and links
    QLabel* createProgrammingCredits();

    //sets default stylesheet properties
    void initializeStyleSheet();
};

#endif // CREDITSWIDGET_H

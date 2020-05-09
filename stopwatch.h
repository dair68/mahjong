#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QLabel>
#include <QTimer>

class Stopwatch : public QLabel{
    Q_OBJECT
public:
    //creates a stopwatch
    //@param parent - pointer to parent widget. null pointer by default
    Stopwatch(QWidget* parent = nullptr);

    //resets the time to 0 seconds
    void reset();

    //increments time by 1 second
    void incrementTime();

    //increases the current time by a certain number of seconds
    //@param sec - number of seconds to increase
    void increaseTime(const unsigned sec);

    //returns the time in hh:mm:ss. if hours is 0, returns mm:ss or m:ss
    QString getTime() const;

    //updates the time displayed by widget. displays hh:mm:ss.
    void updateDisplay();

public slots:
    //starts the stopwatch. increments time every second and displays hh:mm:ss.
    void start();

    //stops the stopwatch. current time preserved on display
    void stop();

private:
    unsigned seconds;
    QTimer timer;
};

#endif // STOPWATCH_H

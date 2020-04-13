#include "stopwatch.h"
#include <QTimer>
#include <QPainter>

Stopwatch::Stopwatch(QWidget* parent) : QLabel(parent), seconds(0), timer(this) {
    connect(&timer, &QTimer::timeout, this, &Stopwatch::incrementTime);
    updateDisplay();
}

void Stopwatch::start() {
    const int milliseconds = 1000;
    timer.start(milliseconds);
}

void Stopwatch::stop() {
    timer.stop();
}

void Stopwatch::reset() {
    seconds = 0;
    updateDisplay();
}

void Stopwatch::incrementTime() {
    seconds++;
    updateDisplay();
}

void Stopwatch::increaseTime(const unsigned sec) {
    seconds += sec;
    updateDisplay();
}

QString Stopwatch::getTime() const {
    const unsigned hr = seconds / 3600;
    const unsigned min = (seconds % 3600) / 60;
    const unsigned sec = seconds % 60;

    const QString hrString = (hr == 0) ? "" : QString::number(hr);
    const QString minString = (hr > 0 && min < 10) ? "0" + QString::number(min) : QString::number(min);
    const QString secString = (sec < 10) ? "0" + QString::number(sec) : QString::number(sec);

    return (hrString == "") ? minString + ":" + secString : hrString + ":" + minString + ":" + secString;
}

void Stopwatch::updateDisplay() {
    QString time = getTime();
    setText("Time\n" + time);
}

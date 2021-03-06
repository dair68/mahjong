#include "shisenmenu.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

ShisenMenu::ShisenMenu(QWidget* parent) : QMenuBar(parent) {
    QMenu* menu = addMenu("Menu");

    QAction* help = menu->addAction("Help");
    connect(help, SIGNAL(triggered()), this, SIGNAL(helpSelected()));

    QAction* restart = menu->addAction("Restart game");
    connect(restart, SIGNAL(triggered()), this, SLOT(showRestartDialog()));

    QAction* newGame = menu->addAction("New game");
    connect(newGame, SIGNAL(triggered()), this, SLOT(showNewGameDialog()));

    QAction* title = menu->addAction("Return to title");
    connect(title, SIGNAL(triggered()), this, SLOT(showTitleDialog()));

    QAction* quit = menu->addAction("Quit");
    connect(quit, SIGNAL(triggered()), this, SLOT(showQuitDialog()));
}

void ShisenMenu::showRestartDialog() const {
    QMessageBox dialog;
    emit modalCreated();
    dialog.setText("Restart the current game?");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = dialog.exec();

    //different button cases
    switch (ret) {
      //yes clicked, restarting game
      case QMessageBox::Yes:
          emit restartSelected();
          break;
      //no clicked, resuming game
      case QMessageBox::No:
          emit noSelected();
          break;
      //default case. error occured
      default:
          qDebug() << "invalid response";
          break;
    }
}

void ShisenMenu::showNewGameDialog() const {
    QMessageBox dialog;
    emit modalCreated();
    dialog.setText("Start a new game?");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = dialog.exec();

    //different button cases
    switch (ret) {
      //yes clicked, restarting game
      case QMessageBox::Yes:
          emit newGameSelected();
          break;
      //no clicked, resuming game
      case QMessageBox::No:
          emit noSelected();
          break;
      //default case. error occured
      default:
          qDebug() << "invalid response";
          break;
    }
}

void ShisenMenu::showQuitDialog() const {
    QMessageBox dialog;
    emit modalCreated();
    dialog.setText("Are you sure you want to quit?");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = dialog.exec();

    //different button cases
    switch (ret) {
      //yes clicked, restarting game
      case QMessageBox::Yes:
          QApplication::quit();
          break;
      //no clicked, resuming game
      case QMessageBox::No:
          emit noSelected();
          break;
      //default case. error occured
      default:
          qDebug() << "invalid response";
          break;
    }
}

void ShisenMenu::showTitleDialog() const {
    QMessageBox dialog;
    emit modalCreated();
    dialog.setText("Return to title screen?");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = dialog.exec();

    //different button cases
    switch (ret) {
      //yes clicked, restarting game
      case QMessageBox::Yes:
          emit returnToTitleSelected();
          break;
      //no clicked, resuming game
      case QMessageBox::No:
          emit noSelected();
          break;
      //default case. error occured
      default:
          qDebug() << "invalid response";
          break;
    }
}

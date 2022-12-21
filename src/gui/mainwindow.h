#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

class Game;
class Engine;


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(Game *game, Engine *engine);
};


#endif // MAINWINDOW_H

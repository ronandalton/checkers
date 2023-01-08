#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QMainWindow>

class Game;
class Engine;


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(Game *game, Engine *engine);
};


#endif // MAIN_WINDOW_H

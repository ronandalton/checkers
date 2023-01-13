#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include "gui/game_manager.h"

#include <QMainWindow>


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();

private:
	GameManager m_game_manager;
};


#endif // MAIN_WINDOW_H

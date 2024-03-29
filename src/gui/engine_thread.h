#ifndef ENGINE_THREAD_H
#define ENGINE_THREAD_H


#include "gui/engine_thread_controller.h"
#include "engine/engine.h"

#include <QThread>


class GuiGameData;
class Game;
class Board;


class EngineThread : public QThread {
	Q_OBJECT

public:
	EngineThread(GuiGameData *gui_game_data, QObject *parent = nullptr);

public slots:
	void makeMovePerhaps();

signals:
	void engineMoveMade();

protected:
	//void run() override; // not used to event loop is started by default

private slots:
	void makeMove(const Move &move);

private:
	Game *m_game;
	Board *m_board;

	Engine m_engine;
	EngineThreadController m_engine_thread_controller;
};


#endif // ENGINE_THREAD_H

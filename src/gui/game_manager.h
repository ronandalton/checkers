#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include "game/game.h"
#include "engine/engine.h"
#include "game/board.h"

#include <QThread>


class GameManager {
public:
	void startGame();

	Game& getGameRef();
	Board& getBoardRef();

private:
	class RunnerThread : public QThread {
		Q_OBJECT

	public:
		void run() override;

	public signals:
		void gameOver();
	};

	Move getMoveFromHuman();
	Move getMoveFromEngine();

	RunnerThread m_runner_thread;

	Game m_game;
	Engine m_engine;
	Board m_board;
};


#endif // GAME_MANAGER_H

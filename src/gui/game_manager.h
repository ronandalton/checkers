#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include "gui/gui_game_data.h"
#include "gui/engine_thread.h"

#include <QObject>
#include <QThread>


class InputHandler;


class GameManager : public QObject {
	Q_OBJECT

public:
	GameManager();

	void startGame();
	GuiGameData* getGuiGameDataPtr();
	EngineThread* getEngineThreadPtr();

signals:
	void gameStarted();

private:
	GuiGameData m_gui_game_data;
	EngineThread m_engine_thread;
};


#endif // GAME_MANAGER_H

#include "gui/game_manager.h"

#include "game/player.h"
#include "game/move.h"


GameManager::GameManager() :
	m_engine_thread(&m_gui_game_data)
{
	connect(this, &GameManager::gameStarted, &m_engine_thread,
		&EngineThread::makeMovePerhaps, Qt::QueuedConnection);
	connect(&m_engine_thread, &EngineThread::engineMoveMade,
		&m_engine_thread, &EngineThread::makeMovePerhaps, Qt::QueuedConnection);
}


void GameManager::startGame() {
	m_gui_game_data.game.newGame(MatchType::HUMAN_VS_COMPUTER);
	m_gui_game_data.board = m_gui_game_data.game.getBoard();
	emit gameStarted();
}


GuiGameData* GameManager::getGuiGameDataPtr() {
	return &m_gui_game_data;
}


EngineThread* GameManager::getEngineThreadPtr() {
	return &m_engine_thread;
}

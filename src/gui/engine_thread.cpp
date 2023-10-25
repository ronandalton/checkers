#include "gui/engine_thread.h"

#include "gui/engine_thread_controller.h"
#include "gui/engine_thread_worker.h"
#include "gui/gui_game_data.h"
#include "game/move.h"
#include "game/player.h"


EngineThread::EngineThread(GuiGameData *gui_game_data, QObject *parent) :
	QThread(parent),
	m_game(&gui_game_data->game),
	m_board(&gui_game_data->board),
	m_engine_thread_controller(&m_engine)
{
	qRegisterMetaType<Game>("Game");
	qRegisterMetaType<Move>("Move");

	connect(&m_engine_thread_controller, &EngineThreadController::finishedProcessing, this, &EngineThread::makeMove);
}


void EngineThread::makeMovePerhaps() {
	if (!m_game->isOver()) {
		if (m_game->getPlayerType(m_game->getTurn()) == Player::COMPUTER) {
			//Move best_move = m_engine.findBestMove(*m_game);
			m_engine_thread_controller.operate(*m_game);
		}
	}

}


void EngineThread::makeMove(const Move &move) {
	m_game->doMove(move);
	*m_board = m_game->getBoard();
	
	emit(engineMoveMade());
}

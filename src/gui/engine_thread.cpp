#include "gui/engine_thread.h"

#include "gui/gui_game_data.h"
#include "game/move.h"
#include "game/player.h"


EngineThread::EngineThread(GuiGameData *gui_game_data, QObject *parent) :
	QThread(parent),
	m_game(&gui_game_data->game),
	m_board(&gui_game_data->board)
{}


void EngineThread::makeMovePerhaps() {
	if (!m_game->isOver()) {
		if (m_game->getPlayerType(m_game->getTurn()) == Player::COMPUTER) {
			msleep(250);

			Move best_move = m_engine.findBestMove(*m_game);
			m_game->doMove(best_move);
			*m_board =  m_game->getBoard();
			
			emit(engineMoveMade());
		}
	}

}

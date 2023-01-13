#include "gui/game_manager.h"

#include "game/player.h"
#include "game/move.h"


void GameManager::runGameLoop() {
	m_game.newGame(MatchType::HUMAN_VS_COMPUTER);
	
	while (!m_game.isOver()) {
		Move move;

		if (m_game.getPlayerType(m_game.getTurn()) == Player::HUMAN) {
			move = getMoveFromHuman();
		} else {
			move = getMoveFromEngine();
		}

		m_game.doMove(move);
	}
}

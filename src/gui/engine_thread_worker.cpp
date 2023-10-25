#include "gui/engine_thread_worker.h"

#include "game/game.h"
#include "engine/engine.h"

#include <QThread>


void EngineThreadWorker::findBestMove(const Game &game) {
	Move best_move = m_engine->findBestMove(game);

	emit bestMoveFound(best_move);
}

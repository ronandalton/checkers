#ifndef ENGINE_THREAD_WORKER_H
#define ENGINE_THREAD_WORKER_H


#include "engine/engine.h"

#include <QThread>


class Game;
class Move;


class EngineThreadWorker : public QObject {
	Q_OBJECT

public:
	EngineThreadWorker(Engine *engine) : m_engine(engine)
	{}

public slots:
	void findBestMove(const Game &game);

signals:
	void bestMoveFound(const Move &best_move);

private:
	Engine *m_engine;
};


#endif // ENGINE_THREAD_WORKER_H

#ifndef ENGINE_THREAD_CONTROLLER_H
#define ENGINE_THREAD_CONTROLLER_H


#include <QObject>
#include <QThread>


class Game;
class Engine;
class Move;


class EngineThreadController : public QObject {
	Q_OBJECT

	QThread workerThread;

public:
	EngineThreadController(Engine *engine);

	~EngineThreadController();

public slots:
	void handleResults(const Move &move);

signals:
	void operate(const Game &game);
	void finishedProcessing(const Move &move);
};


#endif // ENGINE_THREAD_CONTROLLER_H

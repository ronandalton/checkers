#include "gui/engine_thread_controller.h"

#include "gui/engine_thread_worker.h"


EngineThreadController::EngineThreadController(Engine *engine) {
    EngineThreadWorker *worker = new EngineThreadWorker(engine);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &EngineThreadController::operate, worker, &EngineThreadWorker::findBestMove);
    connect(worker, &EngineThreadWorker::bestMoveFound, this, &EngineThreadController::handleResults);
    workerThread.start();
}


EngineThreadController::~EngineThreadController() {
    workerThread.quit();
    workerThread.wait();
}


void EngineThreadController::handleResults(const Move &move) {
    emit finishedProcessing(move);
}

#include "controller.h"

Controller::Controller() {
    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::generate, worker, &Worker::doGenerate);
    connect(this, &Controller::translate, worker, &Worker::doTranslate);
    workerThread.start();
    qDebug() << " Thread started";
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

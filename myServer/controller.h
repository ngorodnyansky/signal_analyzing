#pragma once
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "worker.h"

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    ~Controller();
signals:
    void generate(qintptr socketDescriptor);
    void translate(qintptr socketDescriptor, QVector<double> points);
};

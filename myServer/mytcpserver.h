#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include "controller.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    void startServer();
};




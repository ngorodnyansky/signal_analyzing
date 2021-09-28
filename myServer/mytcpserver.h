#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

class MyTcpServer : public QTcpServer
{
public:
    void startServer();
    virtual void incomingConnection(qintptr socketDescriptor)=0;
};
class GenerationServer : public MyTcpServer
{
    Q_OBJECT
protected:
    void incomingConnection(qintptr socketDescriptor);
};




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
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    void startServer();
    double amplitude=1;
    int speed = 90000;

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QTcpServer *server;
    QTcpSocket *socket;
};




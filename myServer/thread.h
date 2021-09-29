#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(qintptr ID, QObject *parent = 0);
    double amplitude=1;
    int frequency = 10;
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
};




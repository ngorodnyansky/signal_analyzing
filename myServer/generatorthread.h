#pragma once
#include <QThread>
#include <QTcpSocket>


class GeneratorThread:public QThread
{
    Q_OBJECT
public:
    explicit GeneratorThread(qintptr ID, QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    double amplitude=1;
    int frequency = 10;
    QTcpSocket *socket;
    qintptr socketDescriptor;
};


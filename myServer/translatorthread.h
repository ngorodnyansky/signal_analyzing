#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QDataStream>


class TranslatorThread:public QThread
{
    Q_OBJECT
public:
    explicit TranslatorThread(QString fileName,qintptr ID, QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    int i=0;
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QVector <double> translatorData;
};


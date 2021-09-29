#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>


class Worker : public QObject
{
    Q_OBJECT

public slots:
    void readyRead();
    void doGenerate(qintptr ID);
    void doTranslate(qintptr ID,QVector <double> points);
signals:
    void error(QTcpSocket::SocketError socketerror);
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    double amplitude=1;
    int frequency = 10;
};




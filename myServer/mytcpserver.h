#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QString>

class MyTcpServer : public QTcpServer
{
public:
    explicit MyTcpServer(const QString& dataFileName);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);
    QString m_dataFileName;
};
class GenerationServer : public MyTcpServer
{
    Q_OBJECT
protected:
    void incomingConnection(qintptr socketDescriptor);
};






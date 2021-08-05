#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <cmath>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!server->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}
void MyTcpServer::slotNewConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

}

void MyTcpServer::slotServerRead()
{
    QVector<double> array;
    QDataStream in(socket);
    in >> array;
    amplitude=array[1]*0.1;
    speed=array[2];
    double xBegin=array[0], X=xBegin, data=0;
    if(X==6.28){
        X=0;
    }
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if(connected){
    data=amplitude*sin(X*0.2*3.14159265*speed);
    socket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
    socket->waitForBytesWritten();
    QThread::currentThread()->usleep(50000);
    }
}

void MyTcpServer::slotClientDisconnected()
{
    socket->close();
}

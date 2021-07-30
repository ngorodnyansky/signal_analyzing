#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

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
    speed=1100000-array[2];
    double xBegin=array[0], X=xBegin, data=0;
    if(X>6.3){
        while(X>6.3){
            X-=6.3;
        }
    }
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if(connected){
    data=amplitude*sin(X);
    socket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
    socket->waitForBytesWritten();
    qDebug() << data;
    QThread::currentThread()->usleep(speed);
    }
}

void MyTcpServer::slotClientDisconnected()
{
    socket->close();
}

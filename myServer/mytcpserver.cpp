#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QThread>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}
void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

}

void MyTcpServer::slotServerRead()
{
    QVector<double> array;
    QDataStream in(mTcpSocket);
    in >> array;
    amplitude=array[1]*0.1;
    speed=1100000-array[2];
    double xBegin=array[0], X=xBegin, data=0;
    if(X>6.3){
        while(X>6.3){
            X-=6.3;
        }
    }
    bool connected = (mTcpSocket->state() == QTcpSocket::ConnectedState);
    if(connected){
    data=amplitude*sin(X);
    mTcpSocket->write(QByteArray::fromStdString(QVariant(data).toString().toStdString()));
    mTcpSocket->waitForBytesWritten();
    qDebug() << data;
    QThread::currentThread()->usleep(speed);
    }
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}
